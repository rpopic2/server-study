#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>

void throw(void);
int main(void)
{
    int socket_fd;
    socket_fd = socket(PF_INET, SOCK_STREAM, 0);

    if (errno)
    {
        char message[] = "error occured while creating socket";
        write(2, message, sizeof(message));
        exit(errno);
    }

    close(socket_fd);
}
