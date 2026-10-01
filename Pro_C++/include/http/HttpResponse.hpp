#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <string>
#include <map>

namespace Http {

class HttpResponse {
public:
    int statusCode;
    std::string statusMessage;
    std::string contentType;
    std::map<std::string, std::string> headers;
    std::string body;

    HttpResponse(int code = 200, std::string message = "OK",
                 std::string type = "text/plain", std::string content = "");

    void setHeader(const std::string& key, const std::string& value);
    std::string toString() const;

    static HttpResponse html(const std::string& content, int status = 200);
    static HttpResponse json(const std::string& content, int status = 200);
    static HttpResponse plain(const std::string& content, int status = 200);
    static HttpResponse error(int status, const std::string& message);
    static HttpResponse corsPreflight();

private:
    static std::string getDefaultStatusMessage(int code);
};

} // namespace Http

#endif // HTTP_RESPONSE_HPP

