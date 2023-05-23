#include "HTTPResponse.h"
#include <cstdint>
#include <string>

static constexpr auto bad_reponse_400_body = "<html>\
<head><title>400 Bad Request</title></head>\
<body bgcolor=\"white\">\
<center><h1>400 Bad Request</h1></center>\
</body>\
</html>";


void HTTPResponse::generate_response() {
    if (true) {
    //if (!_request.valid()) {
        // Set fields for 400 response
        std::map<std::string, std::string> headers{};
        compose_response(StatusCode::bad_request, headers, bad_reponse_400_body);
        return;
    }
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
