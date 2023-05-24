#include <boost/program_options/options_description.hpp>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <filesystem>

#include <boost/program_options.hpp>
#include <vector>

#include "HTTPHandler.h"
#include "ASIOGenericServer.h"


int main(int argc, char**argv) {
    boost::program_options::options_description desc("Mu HTTP - A tiny high performance HTTP server\nHelp");
    desc.add_options()
        ("help", "Display this Message")
        ("web-root", boost::program_options::value<std::string>()->default_value("/opt/"), 
         "The web root that the server should use.") 
        ("ip", boost::program_options::value<std::string>()->default_value("0.0.0.0"),
         "The IP address on which to listen for connections.") 
        ("port", boost::program_options::value<uint16_t>()->default_value(8080), 
         "The port on which to listen for connections.")
        ("threads", boost::program_options::value<uint16_t>()->default_value(1), 
         "The number of threads to be used to handle connections.");

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return EXIT_SUCCESS;
    }

    const auto web_root = vm["web-root"].as<std::string>();
    const auto ip = vm["ip"].as<std::string>();
    const auto port = vm["port"].as<uint16_t>();
    const auto threads = vm["threads"].as<uint16_t>();

    // Set pwd to web root
    if (std::filesystem::exists(web_root) && std::filesystem::is_directory(web_root)) {
        std::filesystem::current_path(web_root);
    }
    else {
        std::cout << "[!] Specified web root (" << web_root << ") is invalid, terminating." << std::endl;
        return EXIT_FAILURE;
    }

    ASIOGenericServer<HTTPHandler> server(threads);
    server.start_server(ip, port);
    return 0;
}

