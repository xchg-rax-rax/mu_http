#pragma once

#include <string>

#include "HTTPRequest.h"

enum class StatusCode: uint32_t {
    // 2xx Success
    ok = 200,
    // 4xx Client Error
    bad_request = 400,
    not_found = 404,
    internal_server_error = 500,
    not_implemented = 501,
    http_version_not_supported = 505
};

const static std::unordered_map<StatusCode, std::string> status_code_to_reason_phrase {
    {StatusCode::ok, "OK"},
    {StatusCode::bad_request, "Bad Request"},
    {StatusCode::not_found, "Not Found"},
    {StatusCode::internal_server_error, "Internal Server Error"},
    {StatusCode::not_implemented, "Not Implemented"},
    {StatusCode::http_version_not_supported, "HTTP Version not supported"}
};

class HTTPResponse {
    public:
        HTTPResponse(HTTPRequest request) : _request{std::move(request)}
        {
            generate_response();
        }
        std::string get_response() const {
            return _response;
        }
    private:
        const HTTPRequest _request;
        std::string _response;
        void generate_response();
        void compose_response(StatusCode status_code, std::map<std::string, std::string> headers, std::string_view message_body);
        bool read_target_file(const std::string& target_file_path, std::string& target_file);
        std::string generate_timestamp();
};
