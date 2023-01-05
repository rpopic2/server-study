#include "tcp_socket.h"
#include <iostream>

int main() {
    tcp_socket socket;
    socket.connect(ipaddr::LOOPBACK, port::port(8080));
    std::string msg;
    socket >> msg;
    std::cout << msg;
}
