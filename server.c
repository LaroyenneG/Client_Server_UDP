//
// Created by Guillaume Laroyenne S3B2 on 16/12/16.
//

#include "client_server.h"


const char bonjour[] = "Bonjour ";


int main(int argc, char **argv) {

    if (argc != 2) {
        fprintf(stderr, "Usage : serveur <port>\n");
        exit(EXIT_FAILURE);
    }

    int serverSocket = socket(PF_INET6, SOCK_DGRAM, 0);
    if (serverSocket < 0) {
        perror("socket()");
        exit(EXIT_FAILURE);
    }


    struct sockaddr_in6 sin6;
    socklen_t lensin6 = sizeof(sin6);

    memset(&sin6, 0, lensin6);

    sin6.sin6_addr = in6addr_any;
    sin6.sin6_port = htons((uint16_t) atoi(argv[1]));
    sin6.sin6_family = AF_INET6;


    if (bind(serverSocket, (struct sockaddr *) &sin6, lensin6) < 0) {
        perror("bind()");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    bool run = true;

    while (run) {

        struct sockaddr_in6 clientAddress;

        char message[MESSAGE_SIZE];
        memset(message, '\0', MESSAGE_SIZE);

        ssize_t nbChar = recvfrom(serverSocket, message, MESSAGE_SIZE, 0, (struct sockaddr *) &clientAddress, &lensin6);
        if (nbChar != MESSAGE_SIZE) {
            perror("recvfrom()");
            close(serverSocket);
            exit(EXIT_FAILURE);
        }


        char ipv6[INET6_ADDRSTRLEN];
        memset(ipv6, '\0', sizeof(ipv6));
        inet_ntop(AF_INET6, &clientAddress.sin6_addr, ipv6, sizeof(ipv6));

        printf("[%s]:%d\n", ipv6, clientAddress.sin6_port);

        char answer[MESSAGE_SIZE];
        memset(answer, '\0', MESSAGE_SIZE);
        strcat(answer, bonjour);
        strcat(answer, message);


        if (strncmp(message, "exit()", 5) == 0) {
            run = false;
            strcpy(answer, "server is closed");
        }

        nbChar = sendto(serverSocket, answer, MESSAGE_SIZE, 0, (struct sockaddr *) &clientAddress, lensin6);
        if (nbChar != MESSAGE_SIZE) {
            perror("sendto()");
            close(serverSocket);
            exit(EXIT_FAILURE);
        }


    }

    close(serverSocket);

    return 0;
}
