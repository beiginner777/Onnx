#pragma once

#include <cstdint>
#include <string>

// Forward declaration
namespace asio {
class io_context;
}

/// @brief asio + Boost.Beast HTTP 服务器
/// @note  Day 2 实现：路由 /infer /reload /metrics /health
class HttpServer {
public:
    HttpServer();
    ~HttpServer();

    // TODO Day 2: start(asio::io_context& ioc, uint16_t port);
    // TODO Day 2: stop();

    HttpServer(const HttpServer&)            = delete;
    HttpServer& operator=(const HttpServer&) = delete;

private:
    // TODO Day 2: tcp::acceptor, tcp::socket, etc.
};
