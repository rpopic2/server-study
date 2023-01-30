#include "tcp.h"
#include <arpa/inet.h>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include <limits>
namespace tcp {
    constexpr socklen_t SOCKADDR_SIZE = sizeof(::sockaddr_in);
    constexpr usize READ_MAX = 1 << 16;
    void socket::throw_err() {
        throw std::runtime_error(strerror(errno));
    }
    socket::socket(fd_t __fd) {
        fd = __fd;
    }
    socket::socket() {
        fd = ::socket(PF_INET, SOCK_STREAM, 0);
    }
    socket::~socket() {
        ::close(fd);
    }
    int socket::as_fd() {
        return fd;
    }
    void socket::set_nonblocking() {
        int flag = fcntl(fd, F_GETFL, 0);
        fcntl(fd, F_SETFL, flag | O_NONBLOCK);
    }
    void socket::connect(const char *__addr) {
        tcpip::sockaddr addr_struct(__addr);
        int result =
            ::connect(fd, addr_struct.ptr, SOCKADDR_SIZE);
        if (result == -1) throw_err();
    }
    void socket::bind(const char *__addr) {
        tcpip::sockaddr addr_struct(__addr);
        int result =
            ::bind(fd, addr_struct.ptr, SOCKADDR_SIZE);
        if (result == -1) throw_err();
    }
    void socket::listen() {
        ::listen(fd, 10);
        if (errno) throw_err();
    }
    [[nodiscard]] socket socket::accept() {
        fd_t incoming_fd = ::accept(fd, NULL, NULL);
        if (errno) throw_err();
        return socket(incoming_fd);
    }
    void socket::write(const char *__str) {
        ::write(fd, __str, strlen(__str));
        if (errno) throw_err();
    }
    isize socket::read(char *__str, usize len = READ_MAX) {
        isize bytes_read = ::read(fd, __str, len);
        if (errno) throw_err();
        return bytes_read;
    }
    void socket::safe_write(const char *__str) {
        std::string message = " ";
        message.append(__str);
        char len = message.length();
        message[0] = len;
        write(message.c_str());
    }
    void socket::safe_write_str(std::string __str) {
        safe_write(__str.c_str());
    }
    isize socket::safe_read(char *__str) {
        isize bytes_read;
        do {
            bytes_read = read(__str);
        } while (bytes_read <= 1);
        isize bytes_to_read = __str[0];
        while (bytes_read < bytes_to_read) {
            bytes_read += read(__str);
        }
        if (bytes_read > bytes_to_read) {
            throw std::runtime_error("read past bytes_to_read");
        }
        return bytes_read;
    }
    isize socket::safe_read_str(std::string &__str) {
        static char buf[READ_MAX];
        memset(&buf, 0, READ_MAX);
        isize bytes_read = safe_read(buf);
        __str = buf;
        __str = __str.substr(1, __str.length());
        return bytes_read - 1;
    }
}
namespace tcpip {
    constexpr usize SOCKADDR_IN_SIZE = sizeof(::sockaddr);
    sockaddr::sockaddr(std::string __addr) {
        usize d = __addr.find(":");
        std::string ip_addr = __addr.substr(0, d);
        std::string port = __addr.substr(d + 1);
        ::memset(&addr_in, 0, SOCKADDR_IN_SIZE);
        addr_in.sin_family = AF_INET;
        addr_in.sin_addr.s_addr = inet_addr(ip_addr.c_str());
        addr_in.sin_port = htons(::atoi(port.c_str()));
        ptr = (::sockaddr *)&addr_in;
    }
}
