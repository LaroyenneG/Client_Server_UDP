//
// Created by Guillaume Laroyenne S3B2 on 16/12/16.
//



#include "client_server.h"


int main(int argc, char *argv[]) {


    const int ARGUMENT_NUMBER = 3;

    if (argc < ARGUMENT_NUMBER + 1) {
        fprintf(stderr, "Usage : client <address> <port> <message>\n");
        exit(EXIT_FAILURE);
    }


    size_t sizeMessage = 1;
    for (int a = ARGUMENT_NUMBER - 1; a < argc; a++) {
        sizeMessage += strlen(argv[a]);
        if (a + 1 < argc) {
            sizeMessage++;
        }
    }

    if (sizeMessage >= MESSAGE_SIZE) {
        fprintf(stderr, "Over size (%d char max)\n", MESSAGE_SIZE);
        exit(EXIT_FAILURE);
    }

    char message[MESSAGE_SIZE];
    memset(message, '\0', MESSAGE_SIZE);


    for (int i = ARGUMENT_NUMBER; i < argc; i++) {
        strcat(message, argv[i]);
        if (i + 1 < argc) {
            strcat(message, " ");
        }
    }


    int socketClient = socket(PF_INET6, SOCK_DGRAM, 0);
    if (socketClient < 0) {
        perror("socket()");
        exit(EXIT_FAILURE);
    }

    socklen_t sin6len = sizeof(struct sockaddr_in6);

    struct sockaddr_in6 sin6;
    memset(&sin6, 0, sin6len);
    sin6.sin6_addr = in6addr_any;
    sin6.sin6_port = htons(0);
    sin6.sin6_family = AF_INET6;


    if (bind(socketClient, (const struct sockaddr *) &sin6, sin6len) < 0) {
        perror("bind()");
        close(socketClient);
        exit(EXIT_FAILURE);
    }

    struct addrinfo sainfo, *psinfo;

    memset(&sainfo, 0, sizeof(struct addrinfo));
    memset(&sin6, 0, sin6len);

    sainfo.ai_flags = 0;
    sainfo.ai_family = PF_INET6;
    sainfo.ai_socktype = SOCK_DGRAM;
    sainfo.ai_protocol = IPPROTO_UDP;

    if (getaddrinfo(argv[1], argv[2], &sainfo, &psinfo) < 0) {
        perror("getaddrinfo");
        exit(EXIT_FAILURE);
    }

    ssize_t nbChar = sendto(socketClient, message, MESSAGE_SIZE, 0, psinfo->ai_addr, sin6len);
    if (nbChar != MESSAGE_SIZE) {
        perror("sendto()");
        close(socketClient);
        exit(EXIT_FAILURE);
    }


    char answer[MESSAGE_SIZE];
    memset(answer, '\0', MESSAGE_SIZE);

    nbChar = recvfrom(socketClient, answer, MESSAGE_SIZE, 0, psinfo->ai_addr, &sin6len);
    if (nbChar != MESSAGE_SIZE) {
        perror("recvfrom()");
        close(socketClient);
        exit(EXIT_FAILURE);
    }


    fprintf(stdout, "%s\n", answer);

    freeaddrinfo(psinfo);

    close(socketClient);

    return 0;
}