#include <sys/socket.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PORT 80
#define ADDRESS "0.0.0.0"

void client(int sock);
char *filesInDir();

int main (int argc, char **argv) {
    struct sockaddr_in addr;
    int accepted_socket;
    int listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == listen_socket) {
        perror("Socket can't created!");
        exit(1);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(ADDRESS);
    if (-1 == bind(listen_socket, (struct sockaddr *) &addr, sizeof(addr))) {
        perror("Socket can't bind!");
        exit(1);
    }

    if (-1 == listen(listen_socket, 10)) {
        perror("Socket can't listen!");
        exit(1);
    }

    for (;;) {
        accepted_socket = accept(listen_socket, 0, 0);
        if (-1 == accepted_socket) {
            perror("Socket can't accept!");
            exit(1);
        }
        client(accepted_socket);
    }
}

void client(int sock) {
    while (1) {
        size_t name_len, cmd_len;
        char msg[] = ">> Write your command: ";
        char name_buf[256];
        char cmd_buf[256];
        write(sock, msg, sizeof(msg));
        name_len = read(sock, name_buf, sizeof(name_buf));
        if (-1 == name_len) {
            perror("Read error!");
            exit(1);
        }
        while (name_len > 0 &&
               ('\n' == name_buf[name_len] - 1 ||
                '\r' == name_buf[name_len] - 1)) {
            name_len--;
        }
        name_buf[name_len] = 0;
        if (!strcmp(name_buf, "cmd\r\n")) {
            FILE *file;
            char file_buf[1024];
            char cmd_msg[] = ">> Write your CMD command: ";
            while (1) {
                write(sock, cmd_msg, sizeof(cmd_msg));
                cmd_len = read(sock, cmd_buf, sizeof(cmd_buf));
                if (-1 == cmd_len) {
                    perror("Read error!");
                    exit(1);
                }
                while (cmd_len > 0 &&
                       ('\n' == cmd_buf[cmd_len] - 1 ||
                        '\r' == cmd_buf[cmd_len] - 1)) {
                    cmd_len--;
                }
                cmd_buf[cmd_len] = 0;
                cmd_buf[strlen(cmd_buf) - 2] = 0;
                if (!strcmp(cmd_buf, "exit")) {
                    break;
                }
                system(cmd_buf);
            }
        }

        else if (!strcmp(name_buf, "exit\r\n"))
        {
            break;
        }
        printf(">> USER SEND COMMAND: %s", name_buf);
        printf(">> USER SEND CMD COMMAND: %s\n", cmd_buf);
    }
    close(sock);
    exit(0);
}
