#include <iostream>
#include <string>
#include <memory>
#include "build/_deps/asio-src/asio/include/asio.hpp"
 
// --- MODULE 1: THE HARDENED WORKER SESSION ---
class SecureSession : public std::enable_shared_from_this<SecureSession> {
public:
    SecureSession(asio::ip::tcp::socket socket) 
        : socket_(std::move(socket)) {}

    void start() {
        do_secure_read();
    }

private:
    void do_secure_read() {
        auto self(shared_from_this());
        
        // Native stream resolution: Automatically reads until HTTP headers finish printing
        asio::async_read_until(
            socket_, 
            stream_buffer_, 
            "\r\n\r\n",
            [this, self](std::error_code ec, std::size_t bytes_transferred) {
                if (!ec) {
                    // Safety Ceiling: Drop connection if headers are maliciously large
                    if (stream_buffer_.size() > 4096) {
                        std::cerr << "[SECURITY CRITICAL] Header threshold exceeded! Dropping client.\n";
                        force_close_socket();
                        return;
                    }

                    std::cout << "[SYSTEM] Safe packet compiled cleanly (" << bytes_transferred << " bytes).\n";
                    send_secure_response();
                } else if (ec != asio::error::operation_aborted) {
                    force_close_socket();
                }
            }
        );
    }

    void send_secure_response() {
        // Step 1: Create the safety pointer (This was missing!)
        auto self(shared_from_this());
        
        // Step 2: Build the HTML (This was missing!)
        std::string html_body = 
            "<html>\n"
            "<head><title>Jashan's Core Server</title></head>\n"
            "<body style=\"background-color: #172d4b; color: #00ff00; font-family: monospace; text-align: center; margin-top: 100px;\">\n"
            "    <h1>[ SYSTEM SECURED ]</h1>\n"
            "    <h2>Welcome to the Custom C++ Engine</h2>\n"
            "    <p>This asynchronous server is running natively on Windows.</p>\n"
            "    <hr style=\"width: 50%; border-color: #00ff00;\">\n"
            "    <p>Status: <strong>ONLINE & MONITORING</strong></p>\n"
            "</body>\n"
            "</html>";

        // Step 3: Calculate the dynamic headers (This was missing!)
        std::string full_http_response = 
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: " + std::to_string(html_body.length()) + "\r\n"
            "Connection: close\r\n\r\n" + 
            html_body;

        auto response_ptr = std::make_shared<std::string>(full_http_response);

        // Step 4: Fire it over the socket
        asio::async_write(
            socket_, 
            asio::buffer(*response_ptr),
            [this, self, response_ptr](std::error_code ec, std::size_t) {
                force_close_socket();
            }
        );
    }

    void force_close_socket() {
        std::error_code ignored_ec;
        socket_.shutdown(asio::ip::tcp::socket::shutdown_both, ignored_ec);
        socket_.close(ignored_ec);
    }

    asio::ip::tcp::socket socket_;
    asio::streambuf stream_buffer_; // Asio's optimized internal dynamic buffer management
};

// --- MODULE 2: THE NETWORK ACCEPTOR ENGINE ---
class HttpServer {
public:
    HttpServer(asio::io_context& io_ctx, short port)
        : acceptor_(io_ctx, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {
        std::cout << "[LAUNCH] Low-Latency engine listening on port " << port << "...\n";
        start_accept();
    }

private:
    void start_accept() {
        acceptor_.async_accept(
            [this](std::error_code ec, asio::ip::tcp::socket socket) {
                if (!ec) {
                    std::cout << "[SYSTEM] New client linked seamlessly. Offloading to worker thread.\n";
                    std::make_shared<SecureSession>(std::move(socket))->start();
                }
                start_accept();
            }
        );
    }

    asio::ip::tcp::acceptor acceptor_;
};

// --- MODULE 3: THE EXECUTION ENTRY POINT ---
int main() {
    try {
        asio::io_context io_ctx;
        HttpServer server(io_ctx, 8080);
        io_ctx.run();
    }
    catch (const std::exception& e) {
        std::cerr << "[CRITICAL ENGINE FAILURE]: " << e.what() << "\n";
    }
    return 0;
}