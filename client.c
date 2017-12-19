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

const int sizeMax = 65536;
const int start = 3;
ssize_t nbChar;


int main(int argc, char *argv[]) {

    if (argc < 4) {
        fprintf(stderr, "Usage : client <address> <port> <message>\n");
        exit(EXIT_FAILURE);
    }


    size_t sizeMessage = 1;
    for (int a = start; a < argc; a++) {
        sizeMessage += strlen(argv[a]);
        if (a + 1 < argc) {
            sizeMessage++;
        }
    }
    if (sizeMessage >= sizeMax) {
        fprintf(stderr, "Over size (%d char max)\n", sizeMax);
        exit(EXIT_FAILURE);
    }

    char message[sizeMessage];
    message[0] = '\0';
    for (int i = start; i < argc; i++) {
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


    int status = bind(socketClient, (const struct sockaddr *) &sin6, sin6len);
    if (status < 0) {
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

    status = getaddrinfo(argv[1], argv[2], &sainfo, &psinfo);
    if (status < 0) {
        perror("getaddrinfo");
        exit(EXIT_FAILURE);
    }

    nbChar = sendto(socketClient, message, sizeMessage, 0, psinfo->ai_addr, sin6len);
    if (nbChar != sizeMessage) {
        fprintf(stderr, "sendto()\n");
        close(socketClient);
        exit(EXIT_FAILURE);
    }


    size_t sizeAnswer = (sizeMessage + 8);
    char answer[sizeAnswer];

    nbChar = recvfrom(socketClient, answer, sizeAnswer, 0, psinfo->ai_addr, &sin6len);
    if (nbChar != sizeAnswer) {
        fprintf(stderr, "recvfrom() : invalid size\n");
        close(socketClient);
        exit(EXIT_FAILURE);
    }


    fprintf(stdout, "%s\n", answer);

    close(socketClient);

    return 0;
}