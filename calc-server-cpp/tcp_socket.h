#pragma once
#include <sstream>
#include <string>
#include <unistd.h>
#include <netinet/ip.h>
#include <errno.h>
#include <stdexcept>
#include <cstring>
using fd_t = int;
using u32 = u_int32_t;
using u16 = u_int16_t;

namespace ipaddr {
    constexpr u32 ANY = INADDR_ANY;
    constexpr u32 LOOPBACK = htonl(INADDR_LOOPBACK);
    u32 ipaddr(const char *literal);
}
namespace port {
    u16 port(int literal);
}

class tcp_socket {
public:
    tcp_socket();
    ~tcp_socket();
    void bind(u32 addr_net_ordered, u16 port_net_ordered);
    void listen();
    void accept(tcp_socket *client);
    void connect(u32 addr_net_ordered, u16 port_net_ordered);
    friend tcp_socket &operator<<(tcp_socket &sock, std::string msg);
    friend tcp_socket &operator>>(tcp_socket &sock, std::string &msg);
private:
    static const auto BUF_SIZE = 1 << 10;
    fd_t socket_fd;
    char buf[BUF_SIZE];
    void throw_err();
    class sockaddr;
};
class tcp_socket::sockaddr {
public:
    const static auto SIZE = sizeof(::sockaddr);
    sockaddr(u32 addr_net_ordered, u16 port_net_ordered);
    ::sockaddr *p;
private:
    sockaddr_in addr;
};