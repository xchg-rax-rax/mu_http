#include "HTTPRequest.h"

#include <cctype>
#include <stdexcept>
#include <string>
#include <ranges>
#include <iterator>
#include <iostream>
#include <string_view>

#include <boost/algorithm/string.hpp>
#include <unordered_map>

static inline void log_failed_request(const std::string_view message, const std::string_view bad_term) {
    std::cerr << "[!] Failed to parse request: " << message << " (" << bad_term << ")" << std::endl;
}
    
static constexpr std::string_view CRLF = "\r\n";

bool HTTPRequest::parse_request() {
    // might wish to modify this in some way to avoid the copy?
    //const std::string_view packet = boost::algorithm::trim_left_copy(_request);
    //std::cout << packet << std::endl;
    int i = 0;
    bool last_line_crlf = false;
    for (const auto line : std::views::split(_request, CRLF)) {
        const auto line_view = std::string_view(line.begin(), line.end());
        // This logic is FUBAR
        if (i == 0) {
            if (!parse_start_line(line_view)) {
                return false;
            }
        }
        else if (line_view == "") {
            if (last_line_crlf) {
                break;
            }
            last_line_crlf = true;
        }
        else {
            if (!parse_header(line_view)) {
                return false;
            }
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
    try {
        _method = method_to_enum.at(method_view);
    }
    catch (const std::out_of_range& e) {
        _method = HTTPRequestMethod::Unknown;
    }
    return true;
}

// Can implement validation later
bool HTTPRequest::parse_uri(const std::string_view uri_view) {
    _uri = uri_view;
    return true;
}

bool HTTPRequest::parse_http_version(const std::string_view http_version_view) {
    if (http_version_view.size() != 8) {
        log_failed_request("Invalid HTTP version length", std::to_string(http_version_view.size()));
        return false;
    }
    if (!http_version_view.starts_with("HTTP/")) {
        log_failed_request("Invalid HTTP version prefix", http_version_view);
        return false;
    }
    if (!std::isdigit(http_version_view[5]) || http_version_view[6] != '.' || 
        !std::isdigit(http_version_view[7])) {
        log_failed_request("Invalid HTTP version number", http_version_view);
        return false;
    }
    _http_version.first = http_version_view[5] - '0';
    _http_version.second = http_version_view[7] - '0';
    return true;
}

bool HTTPRequest::parse_header(const std::string_view header_view) {
    std::string_view key;
    std::string_view value;
    int count = 0;
    for (const auto section: std::views::split(header_view, ':')) {
        if (count == 0) {
            key = std::string_view(section.begin(), section.end());
        }
        else if (count == 1) {
            value = std::string_view(section.begin(), header_view.end());
        }
        else {
            break;
        }
        count++;
    }
    _headers[key] = value;
    return true;
}

void HTTPRequest::debug_print() const {
    std::cout << "Method: " << enum_to_method.at(_method) << "\n";
    std::cout << "URI: " << _uri << "\n";
    std::cout << "HTTP Version: " << _http_version.first << "." << _http_version.second << "\n";
    std::cout << "Headers: " << "\n";
    for (const auto& header : _headers) {
        std::cout << "\t" <<  header.first << " : " << header.second << _http_version.second << "\n";
    }
    //std::cout << "Message-Body: " << _message_body << std::endl;
}
