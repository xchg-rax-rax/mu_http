#include "HTTPRequest.h"

#include <stdexcept>
#include <string>
#include <ranges>
#include <iterator>
#include <iostream>
#include <string_view>

#include <boost/algorithm/string.hpp>
#include <unordered_map>

    
static constexpr std::string_view CRLF = "\r\n";

bool HTTPRequest::parse_request() {
    // might wish to modify this in some way to avoid the copy?
    //const std::string_view packet = boost::algorithm::trim_left_copy(_request);
    //std::cout << packet << std::endl;
    int i = 0;
    for (const auto line : std::views::split(_request, CRLF)) {
        const auto line_view = std::string_view(line.begin(), line.end());
        if (i == 0 && !parse_start_line(line_view)) {
            return false;
        }
        i++;
    }
    return true;
}

// rename to parse_request_line
bool HTTPRequest::parse_start_line(const std::string_view request_line_view) {
    // Request-Line   = Method SP Request-URI SP HTTP-Version CRLF
    int i = 0;
    for (const auto token: std::views::split(request_line_view, ' ')) {
        const auto token_view = std::string_view(token.begin(), token.end());
        std::cout << token_view << std::endl;
        switch(i) {
            case 0:
                if(!parse_method(token_view)) {
                    return false;
                }
                break;
            case 1:
                if (!parse_uri(token_view)) {
                    return false;
                }
                break;
            case 2:
                if (!parse_http_version(token_view)) {
                    return false;
                }
                break;
            case 3:
                if (token_view != CRLF) {
                    return false;
                }
                break;
            default:
                return false;
        }
        i++;
    }
    return true;
}

bool HTTPRequest::parse_method(const std::string_view method_view) {
    static const std::unordered_map<std::string_view, HTTPRequestMethod> method_map{
        {"GET", HTTPRequestMethod::GET},
        {"HEADER", HTTPRequestMethod::HEAD}
    };
    try {
        _method = method_map.at(method_view);
    }
    catch (const std::out_of_range& e) {
        std::cerr << "[!] Failed to parse request: Invalid Method (" << method_view << ")" << std::endl;
        return false;
    }
    return true;
}

bool HTTPRequest::parse_uri(const std::string_view uri_view) {
    return true;
}

bool HTTPRequest::parse_http_version(const std::string_view http_version_view) {
    return true;
}
