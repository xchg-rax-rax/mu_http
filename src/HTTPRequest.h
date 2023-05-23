#pragma once

#include <string>
#include <utility>
#include <cstdint>
#include <string_view>
#include <map>
#include <unordered_map>

enum class HTTPRequestMethod: int16_t {
    GET = 0,
    HEAD = 1,
    Unknown = -1
};

const std::unordered_map<std::string_view, HTTPRequestMethod> method_to_enum {
    {"GET", HTTPRequestMethod::GET},
    {"HEADER", HTTPRequestMethod::HEAD}
};

const std::unordered_map<HTTPRequestMethod, std::string_view> enum_to_method {
    {HTTPRequestMethod::GET, "GET"},
    {HTTPRequestMethod::HEAD, "HEADER"},
    {HTTPRequestMethod::Unknown, "Unknown"} 
};


class HTTPRequest {
    public:
        HTTPRequest(const std::string request) :  _request{std::move(request)}
        {
            _valid = parse_request();
        }

        bool valid() const { return _valid; }
        const std::pair<uint32_t, uint32_t>& http_version() const { return  _http_version; }
        const std::string_view uri() const { return _uri; }
        
        void debug_print() const;
    private:
        bool _valid = false;
        std::map<std::string_view, std::string_view> _headers = {};
        const std::string _request;
        HTTPRequestMethod _method = HTTPRequestMethod::Unknown;
        std::string_view _uri;
        std::pair<uint32_t, uint32_t> _http_version;
        //std::string_view _message_body;
        bool parse_request();
        bool parse_start_line(const std::string_view);
        bool parse_method(const std::string_view);
        bool parse_uri(const std::string_view uri_view);
        bool parse_http_version(const std::string_view http_version_view);
        bool parse_header(const std::string_view);
};
