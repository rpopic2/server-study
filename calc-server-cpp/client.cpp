#include "lib/tcp.h"
#include <iostream>

int main() {
    tcp::socket sock;
    sock.connect("127.0.0.1:8080");
    std::string buf;

    sock.safe_read(buf);
    std::cout<< buf;

    bool run = true;
    std::string ibuf;
    while (run) {
        std::cin >> ibuf;
        sock.safe_write(ibuf.c_str());
        sock.safe_read(buf);
        std::cout << "[Server echo] "<< buf << std::endl;
    }
}
