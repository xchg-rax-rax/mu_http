#include "HTTPResponse.h"

#include <fstream>
#include <iostream>
#include <cstdint>
#include <iterator>
#include <string>
#include <filesystem>


// These are silly, implement a function to prepare these as need (maybe even at compile time?)
static constexpr auto bad_response_400_body = "<html>\
<head><title>400 Bad Request</title></head>\
<body bgcolor=\"white\">\
<center><h1>400 Bad Request</h1></center>\
</body>\
</html>";

static constexpr auto not_found_404_body = "<html>\
<head><title>404 Not Found</title></head>\
<body bgcolor=\"white\">\
<center><h1>404 Not Found</h1></center>\
</body>\
</html>";

static constexpr auto internal_server_error_500_body= "<html>\
<head><title>500 Internal Server Error</title></head>\
<body bgcolor=\"white\">\
<center><h1>500 Internal Server Error</h1></center>\
</body>\
</html>";

static constexpr auto http_version_not_supported_505_body = "<html>\
<head><title>505 HTTP Version not supported</title></head>\
<body bgcolor=\"white\">\
<center><h1>505 HTTP Version not supported</h1></center>\
</body>\
</html>";


void HTTPResponse::generate_response() {
    std::map<std::string, std::string> headers{};
    if (!_request.valid()) {
        // Set fields for 400 response
        compose_response(StatusCode::bad_request, headers, bad_response_400_body);
        return;
    }
    if (_request.http_version().first != 1 || _request.http_version().second > 1) {
        // Set fields for 505 response
        compose_response(StatusCode::http_version_not_supported, 
                headers, 
                http_version_not_supported_505_body);
        return;
    }
    // Should probably do this using std::filesystem::paths
    // Also there's DEFINITELY directory traversal vulns here
    const static std::string pwd = std::filesystem::current_path();
    std::string target_file_path =  pwd + std::string(_request.uri());
    if (target_file_path.ends_with("/")) {
        target_file_path += "index.html";
    }
    std::cout << target_file_path << std::endl;
    if (!std::filesystem::exists(target_file_path))  {
        compose_response(StatusCode::not_found, headers, not_found_404_body);
        return;
    }
    std::string target_file;
    if (!read_target_file(target_file_path, target_file)) {
        // Server error
        compose_response(StatusCode::internal_server_error, headers, internal_server_error_500_body);
        return;
    }
    compose_response(StatusCode::ok, headers, target_file);
    return;
}

void HTTPResponse::compose_response(StatusCode status_code, 
        std::map<std::string, std::string> headers,
        std::string_view message_body) {
    headers["Content-Length"] = std::to_string(message_body.size());
    _response = "HTTP/1.1 " + std::to_string(static_cast<uint32_t>(status_code)) + " " + status_code_to_reason_phrase.at(status_code) + "\r\n";
    for (const auto& header : headers) {
        _response += header.first + std::string(": ") + header.second + std::string("\r\n");
    }
    _response += "\r\n";
    _response += message_body;
}

bool HTTPResponse::read_target_file(const std::string& target_file_path, std::string& target_file) {
    try {
        std::ifstream target_file_fs(target_file_path);
        if (!target_file_fs.is_open()) {
            throw "File exists but could not be opened.";
        }
        target_file = std::string(std::istreambuf_iterator<char>(target_file_fs), 
                std::istreambuf_iterator<char>());
        target_file_fs.close();
    }
    catch (...) {
        std::cerr << "[!] Read Failed! Don't known why; implement better logging here" << std::endl;
        return false;
    }
    return true;
}
