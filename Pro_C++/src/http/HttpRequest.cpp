#include "http/HttpRequest.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

namespace Http {

std::string HttpRequest::urlDecode(const std::string& in) {
    std::string out;
    out.reserve(in.length());
    for (size_t i = 0; i < in.length(); ++i) {
        if (in[i] == '%') {
            if (i + 2 < in.length()) {
                int hexVal = 0;
                std::istringstream hexStream(in.substr(i + 1, 2));
                if (hexStream >> std::hex >> hexVal) {
                    out += static_cast<char>(hexVal);
                    i += 2;
                } else {
                    out += '%';
                }
            } else {
                out += '%';
            }
        } else if (in[i] == '+') {
            out += ' ';
        } else {
            out += in[i];
        }
    }
    return out;
}

std::string HttpRequest::getQueryParam(const std::string& key, const std::string& defaultValue) const {
    auto it = queryParams.find(key);
    if (it != queryParams.end()) {
        return it->second;
    }
    return defaultValue;
}

std::string HttpRequest::getHeader(const std::string& key) const {
    std::string lowerKey = key;
    std::transform(lowerKey.begin(), lowerKey.end(), lowerKey.begin(),
                   [](unsigned char c){ return std::tolower(c); });

    for (const auto& pair : headers) {
        std::string h = pair.first;
        std::transform(h.begin(), h.end(), h.begin(),
                       [](unsigned char c){ return std::tolower(c); });
        if (h == lowerKey) {
            return pair.second;
        }
    }
    return "";
}

HttpRequest HttpRequest::parse(const std::string& raw) {
    HttpRequest req;
    if (raw.empty()) return req;

    size_t headerEnd = raw.find("\r\n\r\n");
    std::string headerSection;
    if (headerEnd != std::string::npos) {
        headerSection = raw.substr(0, headerEnd);
        req.body = raw.substr(headerEnd + 4);
    } else {
        headerSection = raw;
    }

    std::istringstream stream(headerSection);
    std::string requestLine;
    if (std::getline(stream, requestLine)) {
        // Remove trailing \r if any
        if (!requestLine.empty() && requestLine.back() == '\r') {
            requestLine.pop_back();
        }

        std::istringstream lineStream(requestLine);
        std::string rawUri, httpVersion;
        lineStream >> req.method >> rawUri >> httpVersion;

        // Split rawUri into path and query string
        size_t queryPos = rawUri.find('?');
        if (queryPos != std::string::npos) {
            req.path = rawUri.substr(0, queryPos);
            req.queryString = rawUri.substr(queryPos + 1);

            // Parse query parameters
            std::istringstream qStream(req.queryString);
            std::string pair;
            while (std::getline(qStream, pair, '&')) {
                if (pair.empty()) continue;
                size_t eqPos = pair.find('=');
                if (eqPos != std::string::npos) {
                    std::string k = urlDecode(pair.substr(0, eqPos));
                    std::string v = urlDecode(pair.substr(eqPos + 1));
                    req.queryParams[k] = v;
                } else {
                    req.queryParams[urlDecode(pair)] = "";
                }
            }
        } else {
            req.path = rawUri;
        }
    }

    // Parse headers
    std::string headerLine;
    while (std::getline(stream, headerLine)) {
        if (!headerLine.empty() && headerLine.back() == '\r') {
            headerLine.pop_back();
        }
        if (headerLine.empty()) continue;

        size_t colonPos = headerLine.find(':');
        if (colonPos != std::string::npos) {
            std::string key = headerLine.substr(0, colonPos);
            std::string value = headerLine.substr(colonPos + 1);
            // Trim leading space in value
            while (!value.empty() && std::isspace(value.front())) {
                value.erase(value.begin());
            }
            req.headers[key] = value;
        }
    }

    return req;
}

} // namespace Http

