#include "tcp_socket.h"

int main() {
    tcp_socket socket;
    socket.bind(ipaddr::ANY, port::port(8080));
    socket.listen();

    tcp_socket client;
    socket.accept(&client);
    client << "Hello client!";
}
