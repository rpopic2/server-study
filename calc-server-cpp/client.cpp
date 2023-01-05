#include "tcp_socket.h"
#include <iostream>

int main() {
    std::cout << "Connecting to the server..." << std::endl;
    tcp_socket socket;
    socket.connect(ipaddr::LOOPBACK, port::port(8080));
    socket << "Client connected\n";

    while (true) {
        socket >> std::cout;
        std::cin >> socket;

        std::string msg;
        socket >> msg;
        if (msg == "exit") return 0;
        std::cout << "[Server] " << msg << std::endl;
    }
}
