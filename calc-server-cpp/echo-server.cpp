#include "lib/tcp.h"
#include <iostream>

int main() {
    std::cout << "Starting echo server\n";
    tcp::socket sock;
    sock.bind("0.0.0.0:8080");
    sock.listen();
    tcp::socket client = sock.accept();
    std::string buf;

    client.safe_write("Welcome to echo server!!\n");

    bool run = true;
    while (run) {
        if (client.safe_read_str(buf) == 0) break;
        client.safe_write_str(buf);
    }
}
