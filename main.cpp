#include <iostream>
#include <string>
#include <memory>
#include <asio.hpp>
 
class Httpserver {
public:
    Httpserver(asio::io_context& io_ctx, short port)
      : acceptor_(io_ctx, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {
        start_accept();
    }

private:
    void start_accept() {
        acceptor_.async_accept(
            [this](std::error_code ec, asio::ip::tcp::socket socket) {
                if (!ec) {
                    std::cout << "[SYSTEM] New client connected.\n";
                    std::string body = "Connection : close\r\n\r\nCore server active via Jashan's low latency engine.";
                    auto response = std::make_shared<std::string>(
                        "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(body.size()) + "\r\n\r\n" + body);
                    auto shared_socket = std::make_shared<asio::ip::tcp::socket>(std::move(socket));
                    asio::async_write(*shared_socket, asio::buffer(*response),
                        [shared_socket, response](std::error_code /*ec*/, std::size_t /*length*/) {
                            std::error_code ignored_ec;
                            shared_socket->shutdown(asio::ip::tcp::socket::shutdown_both, ignored_ec);
                        });
                }
                start_accept(); // loop back to accept new connections
            });
    }
    asio::ip::tcp::acceptor acceptor_;
};
int main() {
    try {
        asio::io_context io_ctx;
        Httpserver server(io_ctx, 8080);
        std::cout << "[LAUNCH] Low latency engine listening on port 8080...\n";
        io_ctx.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}