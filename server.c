//
// Created by Guillaume Laroyenne S3B2 on 16/12/16.
//

#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <tkPort.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

const size_t sizeMessage = 65536;
const char bonjour[] = "Bonjour ";
ssize_t nbChar;


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


    int bindReturn = bind(serverSocket, (struct sockaddr *) &sin6, lensin6);
    if (bindReturn < 0) {
        perror("bind()");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }


    struct sockaddr_in6 clientAddress;

    char message[sizeMessage + 1];
    message[sizeMessage] = '\0';
    nbChar = recvfrom(serverSocket, message, sizeMessage, 0, (struct sockaddr *) &clientAddress, &lensin6);
    if (nbChar <= 0) {
        fprintf(stderr, "No byte received\n");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }


    char answer[strlen(message) + strlen(bonjour) + 1];
    answer[0] = '\0';
    strcat(answer, bonjour);
    strcat(answer, message);


    nbChar = sendto(serverSocket, answer, strlen(answer) + 1, 0, (struct sockaddr *) &clientAddress, lensin6);
    if (nbChar != strlen(answer) + 1) {
        fprintf(stderr, "sendto() : invalid size\n");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }


    close(serverSocket);

    return 0;
}
