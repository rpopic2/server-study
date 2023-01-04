#include "echo_server.h"
#include <string.h>

int main() {

    fd_t server_fd = socket(PF_INET, SOCK_STREAM, 0);

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    int tmp_result;
    tmp_result =
        connect(server_fd, (sockaddr *)&server_addr, sizeof(sockaddr));
    if (errno) throw_errmsg("connect");
    puts("Please wait...");

    fdread(server_fd);
    printf("[Server] %s", fdread_buf);
    fdwrite(server_fd, "Client connected\n");

    char ibuf[BUF_SIZE];
    int run = 1;
    while (run && read(0, ibuf, BUF_SIZE)) {
        unsigned int ibuf_len = strlen(ibuf);
        fdwrite(server_fd, ibuf);
        memset(ibuf, 0, BUF_SIZE);
        for (ssize_t c, bytes_read = 0; bytes_read < ibuf_len;) {
            c = fdread(server_fd);
            bytes_read += c;
        }
        if (strcmp(fdread_buf, MAGIC_WORD) == 0) {
            fdwrite(1, "Magic Word recieved\n");
            fdwrite(1, "Server closed\n");
            run = 0;
            break;
        }
        fdwrite(1, "[Server Echo] ");
        fdwrite(1, fdread_buf);
    }

    close(server_fd);
    printf("Socket closed\n");
}
