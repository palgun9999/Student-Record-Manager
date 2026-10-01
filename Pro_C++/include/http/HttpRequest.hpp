#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <string>
#include <map>

namespace Http {

class HttpRequest {
public:
    std::string method;
    std::string path;
    std::string queryString;
    std::map<std::string, std::string> queryParams;
    std::map<std::string, std::string> headers;
    std::string body;

    HttpRequest() = default;

    std::string getQueryParam(const std::string& key, const std::string& defaultValue = "") const;
    std::string getHeader(const std::string& key) const;

    static HttpRequest parse(const std::string& raw);
    static std::string urlDecode(const std::string& in);
};

} // namespace Http

#endif // HTTP_REQUEST_HPP

