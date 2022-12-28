#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#define PORT 8080

/* A TCP/IP client that recieves "Hello Client!" from server.
 * Written using only POSIX syscalls.
 * This programme is only capable of connecting to INADDR_LOOPBACK (localhost)
 */

void throw(void);
int main(void)
{
// 0. socket()
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (errno) throw();

// 1. connect()
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    int result =
        connect(socket_fd
               ,(struct sockaddr *)&server_addr
               ,sizeof(server_addr));
    if (result == -1) throw();

// 2. read() and write()
    const int bufsize = 1 << 8;
    char buf[bufsize];
    read(socket_fd, buf, bufsize);
    if (errno) throw();

    char *msg_prefix = "[Server] ";
    write(1, msg_prefix, strlen(msg_prefix));
    write(1, buf, strlen(buf));
    if (errno) throw();


// 3. close()
   close(socket_fd);

   return 0;
}

void throw(void)
{
    char *msg = "Error occured : ";
    /* /!\ Use perror(msg); instead of following lines! */
    write(2, msg, strlen(msg));
    msg = strerror(errno);
    write(2, msg, strlen(msg));
    exit(errno);
}
