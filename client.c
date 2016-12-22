//
// Created by guillaume on 16/12/16.
//

#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <tkPort.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char* argv[]) {

    if(argc<4){
        perror("Usage : client <address> <port> <message>");
        exit(-1);
    }


    int start=3; //message ecrit a partir du 4eme argument
    ssize_t nbChar;

    //calcul de la taille du message et verification
    size_t sizeMessage=0;
    for(int a=start; a<argc; a++){
        sizeMessage+=strlen(argv[a]);
        if(a+1<argc){
            sizeMessage++;
        }
    }
    if(sizeMessage>=256){
        perror("Over size (256 char max)");
        exit(-1);
    }

    //allocation du message
    char message[sizeMessage];
    memset(message, '\0', sizeof(message));

    //concatenation de tous les arguments
    for(int i=start; i<argc; i++){
        strcat(message, argv[i]);
        if(i+1<argc){
            strcat(message, " ");
        }
    }


    //creation du client udp
    struct sockaddr_in serverAddress, clientAddress;

    int socketClient=socket(AF_INET, SOCK_DGRAM, 0);
    if(socketClient<0){
        perror("socket()");
        exit(EXIT_FAILURE);
    }


    clientAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    memset(clientAddress.sin_zero, 0, sizeof(clientAddress.sin_zero));
    clientAddress.sin_port = htons(0);
    clientAddress.sin_family = AF_INET;
    socklen_t lenClient = sizeof(clientAddress);


    int bindReturn = bind(socketClient, (const struct sockaddr *) &clientAddress, lenClient);

    if (bindReturn < 0){
        perror("bind()");
        close (socketClient);
        exit(EXIT_FAILURE);
    }


    serverAddress.sin_addr.s_addr = inet_addr(argv[1]);
    memset(serverAddress.sin_zero, 0, sizeof(serverAddress.sin_zero));
    serverAddress.sin_port = htons((uint16_t) atoi(argv[2]));
    serverAddress.sin_family = AF_INET;
    socklen_t lenServer = sizeof(serverAddress);


    nbChar=sendto(socketClient, (void *) message, sizeMessage, 0, (struct sockaddr *) &serverAddress, lenServer);

    if (nbChar != (ssize_t)sizeMessage) {
        perror("sendto()");
        close (socketClient);
        exit(EXIT_FAILURE);
    }

    //reception du message du serveur

    size_t sizeAnswer=(strlen(message) + 1 + 8); // 8->"Bonjour "
    char answer[sizeAnswer];

    nbChar=recvfrom(socketClient, (void *) answer, sizeAnswer, 0, (struct sockaddr *) &serverAddress, &lenClient);


    if (nbChar != sizeAnswer){
        perror("sendto() : invalid size");
        close(socketClient);
        exit(EXIT_FAILURE);
    }

    fprintf(stderr, "Message du serveur :\n%s\n", answer);

    close(socketClient);

    return 0;
}