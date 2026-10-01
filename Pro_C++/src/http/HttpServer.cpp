#include "http/HttpServer.hpp"
#include <iostream>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

namespace Http {

HttpServer::HttpServer(int port, std::string host)
    : port(port), host(std::move(host)), listenSocket(INVALID_SOCKET), isRunning(false) {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
}

HttpServer::~HttpServer() {
    stop();
    WSACleanup();
}

Router& HttpServer::getRouter() {
    return router;
}

bool HttpServer::running() const {
    return isRunning;
}

int HttpServer::getPort() const {
    return port;
}

const std::string& HttpServer::getHost() const {
    return host;
}

bool HttpServer::start() {
    if (isRunning) return true;

    listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "[HttpServer] Failed to create socket. Error: " << WSAGetLastError() << std::endl;
        return false;
    }

    BOOL opt = TRUE;
    setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(host.c_str());
    serverAddr.sin_port = htons(static_cast<u_short>(port));

    if (bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "[HttpServer] Failed to bind to " << host << ":" << port
                  << ". Error: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        listenSocket = INVALID_SOCKET;
        return false;
    }

    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "[HttpServer] Failed to listen on socket. Error: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        listenSocket = INVALID_SOCKET;
        return false;
    }

    isRunning = true;
    serverThread = std::thread(&HttpServer::runServerLoop, this);
    return true;
}

void HttpServer::stop() {
    if (!isRunning) return;

    isRunning = false;
    if (listenSocket != INVALID_SOCKET) {
        closesocket(listenSocket);
        listenSocket = INVALID_SOCKET;
    }

    if (serverThread.joinable()) {
        serverThread.join();
    }
}

void HttpServer::runServerLoop() {
    while (isRunning) {
        sockaddr_in clientAddr{};
        int clientLen = sizeof(clientAddr);
        SOCKET clientSocket = accept(listenSocket, (sockaddr*)&clientAddr, &clientLen);

        if (clientSocket == INVALID_SOCKET) {
            if (!isRunning) break;
            continue;
        }

        std::thread([this, clientSocket]() {
            this->handleClient(clientSocket);
        }).detach();
    }
}

void HttpServer::handleClient(SOCKET clientSocket) {
    std::string rawRequest;
    std::vector<char> buffer(4096);
    int totalBytesRead = 0;
    size_t expectedContentLength = 0;
    bool headerParsed = false;

    // Timeout for client socket so thread doesn't hang indefinitely
    DWORD timeout = 4000; // 4 seconds
    setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));

    while (true) {
        int bytesRead = recv(clientSocket, buffer.data(), static_cast<int>(buffer.size()), 0);
        if (bytesRead <= 0) {
            break;
        }
        totalBytesRead += bytesRead;
        rawRequest.append(buffer.data(), bytesRead);

        if (!headerParsed) {
            size_t headerEnd = rawRequest.find("\r\n\r\n");
            if (headerEnd != std::string::npos) {
                headerParsed = true;
                // Check Content-Length header
                std::string headerSection = rawRequest.substr(0, headerEnd);
                std::string clHeader = "Content-Length: ";
                size_t clPos = headerSection.find(clHeader);
                if (clPos == std::string::npos) {
                    clHeader = "content-length: ";
                    clPos = headerSection.find(clHeader);
                }

                if (clPos != std::string::npos) {
                    size_t valStart = clPos + clHeader.length();
                    size_t valEnd = headerSection.find("\r\n", valStart);
                    if (valEnd != std::string::npos) {
                        try {
                            expectedContentLength = std::stoul(headerSection.substr(valStart, valEnd - valStart));
                        } catch (...) {
                            expectedContentLength = 0;
                        }
                    }
                }

                size_t bodyBytes = rawRequest.size() - (headerEnd + 4);
                if (bodyBytes >= expectedContentLength) {
                    break;
                }
            }
        } else {
            size_t headerEnd = rawRequest.find("\r\n\r\n");
            size_t bodyBytes = rawRequest.size() - (headerEnd + 4);
            if (bodyBytes >= expectedContentLength) {
                break;
            }
        }
    }

    if (!rawRequest.empty()) {
        HttpRequest req = HttpRequest::parse(rawRequest);
        HttpResponse res = router.route(req);
        std::string resStr = res.toString();

        size_t totalSent = 0;
        while (totalSent < resStr.size()) {
            int sent = send(clientSocket, resStr.data() + totalSent,
                            static_cast<int>(resStr.size() - totalSent), 0);
            if (sent <= 0) break;
            totalSent += sent;
        }
    }

    shutdown(clientSocket, SD_BOTH);
    closesocket(clientSocket);
}

} // namespace Http

