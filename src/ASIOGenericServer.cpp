#include <memory>
#include <thread>
#include <cstdint>
#include <iterator>
#include <iostream>

using namespace boost::asio;

template <typename ConnectionHandler>
void ASIOGenericServer<ConnectionHandler>::start_server(const std::string& ip, uint16_t port) {
    auto handler = std::make_shared<ConnectionHandler>(_io_service);

    // set up the acceptor to listen on the tcp port
    ip::tcp::endpoint endpoint(ip::address::from_string(ip), port);
    _acceptor.open(endpoint.protocol());
    _acceptor.set_option(ip::tcp::acceptor::reuse_address(true));
    _acceptor.bind(endpoint);
    _acceptor.listen();

    _acceptor.async_accept(handler->socket(),
        [=](auto ec) {
            handle_new_connection(handler, ec);
        }
    );
    
    // start pool of threads to process the asio events
    for (int i = 0; i<_thread_count; i++) {
        _thread_pool.emplace_back(std::thread([=]{_io_service.run();}));
    }
    for (auto& thread : _thread_pool) {
        thread.join();
    }
}


template <typename ConnectionHandler>
void ASIOGenericServer<ConnectionHandler>::handle_new_connection(share_handler_t handler, 
        const boost::system::error_code& error) {
    if (error) { 
        std::cerr << "[!] Connection Failed: " << error.message() << std::endl;
        return; 
    }

    handler->start();
    auto new_handler = std::make_shared<ConnectionHandler>(_io_service);
    _acceptor.async_accept(new_handler->socket(),
        [=](auto ec) {
            handle_new_connection(new_handler, ec);
        }
    );
}
