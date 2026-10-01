#ifndef HTTP_ROUTER_HPP
#define HTTP_ROUTER_HPP

#include "http/HttpRequest.hpp"
#include "http/HttpResponse.hpp"
#include <functional>
#include <map>
#include <string>

namespace Http {

using RouteHandler = std::function<HttpResponse(const HttpRequest&)>;

class Router {
private:
    std::map<std::pair<std::string, std::string>, RouteHandler> routes;

public:
    Router() = default;

    void addRoute(const std::string& method, const std::string& path, RouteHandler handler);
    HttpResponse route(const HttpRequest& request) const;
};

} // namespace Http

#endif // HTTP_ROUTER_HPP

