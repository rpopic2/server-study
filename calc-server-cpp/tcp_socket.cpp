#include "tcp_socket.h"
#include <cstring>
#include <iostream>
#include <stdexcept>
tcp_socket::tcp_socket() {
    socket_fd = ::socket(PF_INET, SOCK_STREAM, 0);
    if (errno) throw std::runtime_error(strerror(errno));
}
tcp_socket::~tcp_socket() {
    ::close(socket_fd);
    std::cout << "Socket closed" << std::endl;
}
void tcp_socket::bind(u32 addr_net_ordered, u16 port_net_ordered) {
    sockaddr_in addr;
    memset(&addr, 0, sizeof(sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = port_net_ordered;
    addr.sin_addr.s_addr = addr_net_ordered;
    int result = ::bind(socket_fd, (::sockaddr *)&addr, sizeof(::sockaddr));
    if (result == -1) throw_err();
}
void tcp_socket::listen() {
    ::listen(socket_fd, 0);
}
void tcp_socket::accept(tcp_socket *client) {
    client->socket_fd = ::accept(socket_fd, NULL, NULL);
    if (errno) throw_err();
}
void tcp_socket::connect(u32 addr_net_ordered, u16 port_net_ordered) {
    tcp_socket::sockaddr addr(addr_net_ordered, port_net_ordered);
    int result = ::connect(socket_fd, addr.p, sockaddr::SIZE);
    if (result == -1) throw_err();
}
tcp_socket &operator<<(tcp_socket &sock, std::string msg) {
    ::write(sock.socket_fd, msg.c_str(), msg.length() + 1);
    return sock;
}
tcp_socket &operator>>(tcp_socket &sock, tcp_socket::prefix prefix) {
    sock._prefix = prefix;
    return sock;
}
tcp_socket &operator>>(tcp_socket &sock, tcp_socket::noprefix __nopre) {
    sock._prefix = tcp_socket::prefix("");
    return sock;
}
tcp_socket &operator>>(tcp_socket &sock, std::string &msg) {
    memset(&sock.rbuf, 0, tcp_socket::BUF_SIZE);
    ::read(sock.socket_fd, sock.rbuf, tcp_socket::BUF_SIZE);
    msg = sock._prefix.str + sock.rbuf;
    return sock;
}
tcp_socket &operator>>(tcp_socket &sock, std::ostream &os) {
    memset(&sock.rbuf, 0, tcp_socket::BUF_SIZE);
    ::read(sock.socket_fd, sock.rbuf, tcp_socket::BUF_SIZE);
    os << sock._prefix.str << sock.rbuf;
    return sock;
}
std::istream &operator>>(std::istream &is, tcp_socket &sock) {
    is.getline(sock.rbuf, tcp_socket::BUF_SIZE);
    sock << sock.rbuf;
    return is;
}

//utils
tcp_socket::sockaddr::sockaddr(u32 addr_net_ordered, u16 port_net_ordered) {
    memset(&addr, 0, sizeof(sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = port_net_ordered;
    addr.sin_addr.s_addr = addr_net_ordered;
    p = (::sockaddr *)&addr;
}
void tcp_socket::throw_err() {
    throw std::runtime_error(strerror(errno));
}
u32 ipaddr::ipaddr(const char *literal) {
    return htonl(atoi(literal));
}
u16 port::port(int literal) {
    return htons(literal);
}
void buf::operator=(std::string s) {
    this->str = s;
}
