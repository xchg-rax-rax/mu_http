#pragma once
#include <string>
#include <thread>
#include <vector>
#include <cstdint>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/system/detail/error_code.hpp>


template<typename ConnectionHandler>
class ASIOGenericServer {
    using share_handler_t = std::shared_ptr<ConnectionHandler>;
    public:
        ASIOGenericServer(int thread_count = 1) 
            : _thread_count{thread_count}
            , _io_service{}
            , _acceptor(_io_service)
        {}
        void start_server(const std::string&, uint16_t port);
    private:
        void handle_new_connection(share_handler_t handler, const boost::system::error_code& error);
        int _thread_count;
        std::vector<std::thread> _thread_pool;
        boost::asio::io_service _io_service;
        boost::asio::ip::tcp::acceptor _acceptor;
};

#include "ASIOGenericServer.cpp"
