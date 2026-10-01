#include "http/Router.hpp"

namespace Http {

void Router::addRoute(const std::string& method, const std::string& path, RouteHandler handler) {
    routes[{method, path}] = std::move(handler);
}

HttpResponse Router::route(const HttpRequest& request) const {
    if (request.method == "OPTIONS") {
        return HttpResponse::corsPreflight();
    }

    auto it = routes.find({request.method, request.path});
    if (it != routes.end()) {
        return it->second(request);
    }

    // Alias /index.html to /
    if (request.path == "/index.html") {
        auto rootIt = routes.find({request.method, "/"});
        if (rootIt != routes.end()) {
            return rootIt->second(request);
        }
    }

    return HttpResponse::error(404, "Endpoint '" + request.method + " " + request.path + "' not found.");
}

} // namespace Http

