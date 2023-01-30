#include "tcp.h"
#include <fcntl.h>
#include <iostream>
#include <ostream>
#include <sys/epoll.h>
#include <vector>
constexpr int NUM_EV = 10;

epoll_event tmp_ev;
int epollfd;

void setnonblocking(fd_t fd) {
    int flag = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}

void handle(fd_t fd);

void add_incoming(fd_t incoming_fd) {
    setnonblocking(incoming_fd);
    tmp_ev.events = EPOLLIN | EPOLLET;
    tmp_ev.data.fd = incoming_fd;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, incoming_fd, &tmp_ev);
    if (errno)
        perror("epoll_ctl, add incoming"), exit(1);
}

int main() {
    std::cout << "Start server" << std::endl;
    tcp::socket tcp_listener;
    tcp_listener.bind("0.0.0.0:801");
    tcp_listener.listen();
    tcp_listener.set_nonblocking();
    fd_t listener_fd = tcp_listener.as_fd();

    std::vector<epoll_event> events(NUM_EV);

    epollfd = epoll_create1(0);
    if (errno)
        perror("epoll create1"), exit(1);

    tmp_ev.events = EPOLLIN;
    tmp_ev.data.fd = listener_fd;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, listener_fd, &tmp_ev);
    if (errno)
        perror("epoll_ctl: ADD"), exit(1);

    while (true) {
        int nfds = epoll_wait(epollfd, events.data(), events.size(), -1);
        if (nfds == -1) perror("epoll_wait"), exit(1);
        std::cout << "Waait" << std::endl;

        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == listener_fd) {
                tcp::socket incoming = tcp_listener.accept();
                if (errno) perror("accept"), exit(1);
                add_incoming(incoming.as_fd());
            } else {
                handle(events[i].data.fd);
            }
        }
    }
}

void handle(fd_t fd) {
    tcp::socket sock(fd);
    std::string buf;
    auto bytes_read = sock.safe_read_str(buf);
    std::cout << "read " << bytes_read << "bytes: " <<buf << std::endl;
}
