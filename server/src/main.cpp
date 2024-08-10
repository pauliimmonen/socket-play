#include "WebSocketServer.hpp"
#include <iostream>
#include <thread>

int main() {
    try {
        WebSocketServer server;
        std::thread server_thread([&server]() {
            server.run(9002);
        });

        std::cout << "Server running. Press 'q' to quit." << std::endl;
        char input;
        do {
            std::cin >> input;
        } while (input != 'q');

        std::cout << "Shutting down server..." << std::endl;
        server.stop();
        server_thread.join();
        std::cout << "Server stopped." << std::endl;
    } catch (websocketpp::exception const & e) {
        std::cout << "WebSocket++ exception: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Standard exception: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "Unknown exception" << std::endl;
    }
    return 0;
}