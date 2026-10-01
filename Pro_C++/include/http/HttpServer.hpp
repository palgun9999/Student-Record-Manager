#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include "http/Router.hpp"
#include <winsock2.h>
#include <atomic>
#include <thread>
#include <string>

namespace Http {

class HttpServer {
private:
    int port;
    std::string host;
    SOCKET listenSocket;
    Router router;
    std::atomic<bool> isRunning;
    std::thread serverThread;

    void runServerLoop();
    void handleClient(SOCKET clientSocket);

public:
    explicit HttpServer(int port = 8080, std::string host = "127.0.0.1");
    ~HttpServer();

    // Prevent copying
    HttpServer(const HttpServer&) = delete;
    HttpServer& operator=(const HttpServer&) = delete;

    Router& getRouter();
    bool start();
    void stop();
    bool running() const;
    int getPort() const;
    const std::string& getHost() const;
};

} // namespace Http

#endif // HTTP_SERVER_HPP

