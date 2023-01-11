#include "lib/tcp.h"
#include <sstream>
#include <string>
#include <iostream>

void send(tcp::socket &sock, std::string s);
int main() {
    tcp::socket socket;
    socket.bind("0.0.0.0:8080");
    socket.listen();
    tcp::socket client = socket.accept();
    client.safe_write_str("Welcome.");
    std::cout << "A client has connected." << std::endl;


    while (true) {
        std::istringstream iss;
        std::string rbuf;
        client.safe_read_str(rbuf);
        std::cout << "[Client] " << rbuf << std::endl;
        if (rbuf == "exit") break;
        iss.seekg(0); 
        iss.str(rbuf);

        int a, b;
        char op;
        int result;
        double result_d;
        iss >> a >> op >> b;
        switch (op) {
            case '+':
                result = a + b;
                break;
            case '-':
                result = a - b;
                break;
            case '*':
                result = a * b;
                break;
            case '/':
                result_d = (double)a / b;
                send(client, std::to_string(result_d));
                continue;
            default:
                client.safe_write_str("Invalid operation " + std::string(1, op));
                continue;
        }
        send(client, std::to_string(result));
        std::cout << iss.str() << std::endl;
    }
}
void send(tcp::socket &sock, std::string s) {
        sock.safe_write_str(s);
}
