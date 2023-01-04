#include "echo_server.h"

int main(void) {
    fd_t server_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (errno) throw_err();

    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int tmp_result;
    tmp_result =
        bind(server_fd, (sockaddr *)&server_addr, sizeof(sockaddr));
    if (tmp_result == -1) throw_err();

    listen(server_fd, 0);
    if (errno) throw_err();

    int run = 1;
    while (run) {
        fd_t client_fd = accept(server_fd, NULL, NULL);
        if (errno) throw_err();
        
        fdwrite(client_fd, "Connected to server.\nSend EOF to disconect.\n");
        fdread(client_fd);
        printf("[Client] %s", fdread_buf);
        while (fdread(client_fd)) {
            printf("[Client] %s", fdread_buf);
            fdwrite(client_fd, fdread_buf);
            if (strcmp(fdread_buf, MAGIC_WORD) == 0) {
                fdwrite(1, "Magic Word recieved\n");
                run = 0;
            }
        }
        close(client_fd);
        printf("Client socket closed\n");
    }
    close(server_fd);
    printf("Socket closed\n");
}
