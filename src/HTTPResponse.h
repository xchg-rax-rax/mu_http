#pragma once

#include <string>

#include "HTTPRequest.h"

enum class StatusCode: uint32_t {
    // 2xx Success
    ok = 200,
    // 4xx Client Error
    bad_request = 400,
    not_found = 404,
    method_not_allowed = 405,
    http_version_not_supported = 505
};

const static std::unordered_map<StatusCode, std::string> status_code_to_reason_phrase {
    {StatusCode::ok, "OK"},
    {StatusCode::bad_request, "Bad Request"},
    {StatusCode::not_found, "Not Found"},
    {StatusCode::method_not_allowed, "Method Not Allowed"},
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
};
