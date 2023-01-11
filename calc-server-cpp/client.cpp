#include "lib/tcp.h"
#include <iostream>

int main(int argc, char *argv[]) {
    tcp::socket socket;
    std::cout << "connecting to the server..." << std::endl;
    const char *addr = argc == 2 ? argv[1] : "127.0.0.1:8080";
    socket.connect(addr);
    std::string rbuf;
    socket.safe_read_str(rbuf);
    std::cout << rbuf << std::endl;
    
    std::string ibuf;

    while (true) {
        std::cout << "calc> ";
        std::getline(std::cin, ibuf);
        if (ibuf.length() == 0) socket.safe_write_str("exit");
        socket.safe_write_str(ibuf);
        if (ibuf == "exit") break;

        socket.safe_read_str(rbuf);
        std::cout << rbuf << std::endl;

    }
}
