#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/ip.h>

#define PORT 8080

/* TCP/IP server that echos "Hello Client!" to client.
 * Written using only POSIX syscalls.
 */

void throw(void);
int main(void)
{
// 1. socket()
    int server_socket_fd
        = socket(PF_INET
                ,SOCK_STREAM
                ,0);
    if (server_socket_fd == -1) throw();

// 2. bind()
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    int result
        = bind(server_socket_fd
              ,(struct sockaddr *)&server_addr
              ,sizeof(server_addr));
    if (result == -1) throw();

// 3. listen()
    result
        = listen(server_socket_fd, 1);
    if (result == -1) throw();
    
// 4. accept()
    int client_socket_fd
        = accept(server_socket_fd
                ,NULL
                ,NULL);
    if (result == -1) throw();

// 5. read() and write()
    const char *message = "Hello Client!";
    write(client_socket_fd, message, strlen(message));

// 6. close()
    close(server_socket_fd);
    close(client_socket_fd);

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
