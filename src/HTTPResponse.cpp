#include "HTTPResponse.h"
#include "HTTPRequest.h"
#include "Mime.h"

#include <set>
#include <iomanip>
#include <chrono>
#include <fstream>
#include <iostream>
#include <cstdint>
#include <iterator>
#include <sstream>
#include <streambuf>
#include <string>
#include <filesystem>


static std::string generate_error_body(const std::string message) {
    const std::string start = "<html>\n<head><title>";
    const std::string middle = "</title></head>\n<body bgcolor=\"white\">\n<center><h1>";
    const std::string end = "</h1></center>\n</body>\n</html>";
    return start + message + middle + message + end;
}

static const auto bad_response_400_body = generate_error_body("400 Bad Request");

static const auto not_found_404_body = generate_error_body("404 Not Found");

static const auto internal_server_error_500_body = generate_error_body("500 Internal Server Error");

static const auto http_version_not_supported_505_body = generate_error_body("505 HTTP Version not supported");

static const auto not_implemented_501_body = generate_error_body("501 Not Implemented");

void HTTPResponse::generate_response() {
    // During optimization should probably be made to be static
    std::map<std::string, std::string> headers{};
    headers["Server"] = "Mu HTTP Server/0.1";
    headers["Date"] = generate_timestamp();

    if (!_request.valid()) {
        compose_response(StatusCode::bad_request, headers, bad_response_400_body);
        return;
    }

    if (_request.http_version().first != 1 || _request.http_version().second > 1) {
        compose_response(StatusCode::http_version_not_supported, 
                headers, 
                http_version_not_supported_505_body);
        return;
    }

    static const std::set<HTTPRequestMethod> allowed_methods {
        HTTPRequestMethod::GET, 
        HTTPRequestMethod::HEAD
    };
    if (allowed_methods.count(_request.method()) == 0) {
        compose_response(StatusCode::not_implemented, headers, not_implemented_501_body);
        return;
    }

    // Should probably do this using std::filesystem::paths
    // std::filesystem seems to be quite good at catching path traversal bugs but best to be safe
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
        compose_response(StatusCode::internal_server_error, headers, internal_server_error_500_body);
        return;
    }
    headers["Content-Type"] = get_mime_type(target_file_path);
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
    if (_request.method() != HTTPRequestMethod::HEAD) {
        _response += message_body;
    }
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

const std::string HTTPResponse::generate_timestamp() {
    auto now = std::chrono::system_clock::now();
    auto now_as_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm *local = std::localtime(&now_as_time_t);
    std::stringstream timestamp_ss;
    timestamp_ss << std::put_time(local, "%a, %d %b %Y %T %Z");
    return timestamp_ss.str();
}

const std::string HTTPResponse::get_mime_type(const std::string& target_file_path) {
    const std::filesystem::path target_path(target_file_path);
    try {
        return mime_map.at(target_path.extension());
    } catch (...) {
        return "application/octet-stream";
    }
}
