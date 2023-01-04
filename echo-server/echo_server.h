#pragma once
#include <errno.h>
#include <netinet/ip.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PORT 8085
#define BUF_SIZE 1024
#define MAGIC_WORD "close_server\n"
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;
typedef int fd_t;
const char *err = "Error";

void throw_errmsg(const char *msg) {
    perror(msg);
    exit(errno);
}
void throw_err(void) {
    throw_errmsg("Error");
}
void fdwrite(fd_t __fd, const char *msg) {
    write(__fd, msg, strlen(msg) + 1);
}
char fdread_buf[BUF_SIZE];
ssize_t fdread(fd_t __fd) {
    return read(__fd, fdread_buf, BUF_SIZE);
}
