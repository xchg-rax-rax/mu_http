#include "HTTPHandler.h"

#include <istream>
#include <cstddef>
#include <iostream>

#include <boost/asio/write.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/system/detail/error_code.hpp>
#include <sstream>

#include "HTTPRequest.h"

using namespace boost;

void HTTPHandler::read_packet() {
    asio::async_read_until(_socket, _in_packet, "\r\n\r\n",
            [me=shared_from_this()](const system::error_code& ec, std::size_t byte_count) {
                me->read_packet_done(ec, byte_count);
            }
    );
}

void HTTPHandler::read_packet_done(const system::error_code& error, std::size_t byte_count) {
    if (error) { 
        std::cerr << "[!] Connection Failed: " << error.message() << std::endl;
        return; 
    }
    std::string packet_string((std::istreambuf_iterator<char>(&_in_packet)), 
                               std::istreambuf_iterator<char>());

    const auto http_request = HTTPRequest(packet_string);
    http_request.debug_print();
    read_packet();
}

void HTTPHandler::send(std::string message) {
    _io_service.post(_write_strand.wrap(
            [me=shared_from_this(), message=std::move(message)]() {
                me->queue_message(std::move(message));
            })
    );
}

void HTTPHandler::queue_message(std::string message) {
    // Note that since we always execute this function from a strand (similar to a serial queue)
    // we can access the std::deque even though isn't thread safe
    bool write_in_progress = !_send_packet_queue.empty();
    _send_packet_queue.push_back(message);
    if (!write_in_progress) {
        start_packet_send();
    }
}

void HTTPHandler::start_packet_send() {
    _send_packet_queue.front() += "\0";
    asio::async_write(_socket, asio::buffer(_send_packet_queue.front()), _write_strand.wrap(
                [me=shared_from_this()](const system::error_code& ec, std::size_t) {
                    me->packet_send_done(ec);
                }
    ));
}

void HTTPHandler::packet_send_done(const system::error_code& error) {
    if (error) {
        return;
    }
    _send_packet_queue.pop_front();
    if(_send_packet_queue.empty()) {
        return;
    }
    start_packet_send(); // if there's still stuff on the queue keep sending it
}
