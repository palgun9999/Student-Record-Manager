#include "http/HttpResponse.hpp"
#include <sstream>

namespace Http {

HttpResponse::HttpResponse(int code, std::string message,
                           std::string type, std::string content)
    : statusCode(code),
      statusMessage(std::move(message)),
      contentType(std::move(type)),
      body(std::move(content)) {
    if (statusMessage.empty()) {
        statusMessage = getDefaultStatusMessage(statusCode);
    }
}

std::string HttpResponse::getDefaultStatusMessage(int code) {
    switch (code) {
        case 200: return "OK";
        case 201: return "Created";
        case 204: return "No Content";
        case 400: return "Bad Request";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        case 500: return "Internal Server Error";
        default: return "Unknown";
    }
}

void HttpResponse::setHeader(const std::string& key, const std::string& value) {
    headers[key] = value;
}

std::string HttpResponse::toString() const {
    std::ostringstream ss;
    ss << "HTTP/1.1 " << statusCode << " " << statusMessage << "\r\n";
    ss << "Content-Type: " << contentType << "\r\n";
    ss << "Content-Length: " << body.size() << "\r\n";
    ss << "Access-Control-Allow-Origin: *\r\n";
    ss << "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n";
    ss << "Access-Control-Allow-Headers: Content-Type, Authorization\r\n";
    ss << "Connection: close\r\n";

    for (const auto& h : headers) {
        ss << h.first << ": " << h.second << "\r\n";
    }

    ss << "\r\n";
    ss << body;
    return ss.str();
}

HttpResponse HttpResponse::html(const std::string& content, int status) {
    return HttpResponse(status, getDefaultStatusMessage(status), "text/html; charset=utf-8", content);
}

HttpResponse HttpResponse::json(const std::string& content, int status) {
    return HttpResponse(status, getDefaultStatusMessage(status), "application/json; charset=utf-8", content);
}

HttpResponse HttpResponse::plain(const std::string& content, int status) {
    return HttpResponse(status, getDefaultStatusMessage(status), "text/plain; charset=utf-8", content);
}

HttpResponse HttpResponse::error(int status, const std::string& message) {
    std::string jsonBody = "{\"error\":\"" + message + "\"}";
    return HttpResponse(status, getDefaultStatusMessage(status), "application/json; charset=utf-8", jsonBody);
}

HttpResponse HttpResponse::corsPreflight() {
    HttpResponse res(204, "No Content", "text/plain", "");
    return res;
}

} // namespace Http

