#pragma once

#include <boost/system/detail/error_code.hpp>
#include <cstddef>
#include <deque>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_service.hpp>

#include <memory>
class HTTPHandler : public std::enable_shared_from_this<HTTPHandler> {
    public:
        HTTPHandler(boost::asio::io_service& service)
            : _io_service(service) 
            , _socket(service)
            , _write_strand(service)
    {}

    boost::asio::ip::tcp::socket& socket() {
        return _socket;
    }

    void start() {
        read_packet();
    }

    void send(const std::string msg);

    private:  
        boost::asio::io_service& _io_service;
        boost::asio::ip::tcp::socket _socket;
        boost::asio::io_service::strand _write_strand;
        boost::asio::streambuf _in_packet;
        std::deque<std::string> _send_packet_queue;

        void read_packet();
        void read_packet_done(const boost::system::error_code& error, std::size_t byte_count);
        void queue_message(const std::string message);
        void start_packet_send();
        void packet_send_done(const boost::system::error_code& error);
};
