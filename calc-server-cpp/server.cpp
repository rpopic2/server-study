#include "lib/tcp.h"
#include <iostream>

int main() {
    tcp::socket sock;
    sock.bind("0.0.0.0:8080");
    sock.listen();
    tcp::socket client = sock.accept();
    std::string buf;

    client.safe_write("Welcome to echo server!\n");

    bool run = true;
    while (run) {
        client.safe_read(buf);
        client.safe_write(buf.c_str());
    }
}
