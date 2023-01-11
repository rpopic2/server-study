#pragma once
#include <string>
#include <netinet/in.h>
using fd_t = int;
using u32 = u_int32_t;
using u16 = u_int16_t;
using usize = size_t;
using isize = ssize_t;

namespace tcp {
    class socket {
        fd_t fd;
        static void throw_err();
        socket(fd_t __fd);
    public:
        socket();
        ~socket();
        void connect(const char *__addr);
        void bind(const char *__addr);
        void listen();
        [[nodiscard]] socket accept();
        void write(const char *__str);
        void safe_write(const char *__str);
        void safe_write_str(std::string __str);
        isize read(char *__str, usize len);
        isize safe_read(char *__str);
        isize safe_read_str(std::string &__str);
    };
}
namespace tcpip {
    class sockaddr {
        ::sockaddr_in addr_in;
    public:
        sockaddr(std::string __addr);
        ::sockaddr *ptr;
    };
}
