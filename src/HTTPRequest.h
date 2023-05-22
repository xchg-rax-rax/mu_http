#pragma once
#include <string>
#include <string_view>
#include <unordered_map>

enum class HTTPRequestMethod: int16_t {
    GET = 0,
    HEAD = 1,
    Unknown = -1
};

class HTTPRequest {
    public:
        HTTPRequest(const std::string request) :  _request{std::move(request)}
        {
            _valid = parse_request();
        }

    private:
        bool _valid = false;
        HTTPRequestMethod _method = HTTPRequestMethod::Unknown;
        std::unordered_map<std::string, std::string> _headers = {};
        const std::string _request;
        bool parse_request();
        bool parse_start_line(const std::string_view);
        bool parse_method(const std::string_view);
        bool parse_uri(const std::string_view uri_view);
        bool parse_http_version(const std::string_view http_version_view);
};
