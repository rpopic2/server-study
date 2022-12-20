#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>

int main(void)
{
    int socket_fd;
    socket_fd = socket(PF_INET, SOCK_STREAM, 100);

    close(socket_fd);

    char buf[2];
    buf[0] = '0' + errno;
    write(1, buf, sizeof(errno_t));
}
