#include "lib/tcp.h"
#include <iostream>

int main() {
    tcp::socket sock;
    std::cout << "connecting...";
    sock.connect("127.0.0.1:8081");
    std::string buf;

    sock.safe_read_str(buf);
    std::cout<< buf;

    bool run = true;
    std::string ibuf;
    while (run) {
        std::getline(std::cin, ibuf);
        sock.safe_write_str(ibuf);
        if (ibuf.length() == 0) break;
        sock.safe_read_str(buf);
        std::cout << "[Server echo] "<< buf << std::endl;
    }
}
