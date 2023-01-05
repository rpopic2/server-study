#include "tcp_socket.h"
#include <ios>
#include <iostream>
#include <sstream>
#include <string>

int main() {
    tcp_socket socket;
    socket.bind(ipaddr::ANY, port::port(8080));
    socket.listen();

    tcp_socket client;
    socket.accept(&client);
    client >> std::cout;

    while (true) {
        client << "calc> ";
        std::string s;
        client >> s;
        if (s == "exit" || s.length() == 0) {
            client << "exit";
            return 0;
        }
        std::istringstream oss(s);
        int a, b; char op;
        oss >> a >> op >> b;
        client << std::to_string(a + b);
        std::cout << (a+b) << std::endl;
    }
}
