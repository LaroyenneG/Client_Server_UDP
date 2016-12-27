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

    if(argc<4) {
        fprintf(stderr, "Usage : client <address> <port> <message>\n");
        exit(EXIT_FAILURE);
    }

    /*
     * variables globales
     */
    const int sizeMax = 65536;    // taille maximale du message
    const int start=3;           //message ecrit a partir du 4eme argument
    ssize_t nbChar;             // pour le nombre d'octets lus et envoyes


    /*
     * preparation du message
     */

    /* calcul de la taille du message et verification */
    size_t sizeMessage=1;   // 1->'\0'
    for(int a=start; a<argc; a++) {
        sizeMessage+=strlen(argv[a]);
        if(a+1<argc) {
            sizeMessage++;
        }
    }
    if(sizeMessage>=sizeMax) {
        fprintf(stderr, "Over size (%d char max)\n", sizeMax);
        exit(EXIT_FAILURE);
    }

    /* concatenation de tous les arguments dans message */
    char message[sizeMessage];
    message[0]='\0';                    // transformation en "string" vide pour strcat()
    for(int i=start; i<argc; i++) {
        strcat(message, argv[i]);
        if(i+1<argc) {
            strcat(message, " ");
        }
    }


    /*
     * configuration pour la transmission udp vers le serveur
     */

    /* creation de la socket du client */
    int socketClient=socket(AF_INET, SOCK_DGRAM, 0);
    if(socketClient<0) {
        perror("socket()");
        exit(EXIT_FAILURE);
    }

    /* creation et configuration de la sockaddr_in du client */
    struct sockaddr_in clientAddress;
    clientAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    memset(clientAddress.sin_zero, 0, sizeof(clientAddress.sin_zero));
    clientAddress.sin_port = htons(0);
    clientAddress.sin_family = AF_INET;
    socklen_t lenClient = sizeof(clientAddress);


    int bindReturn = bind(socketClient, (const struct sockaddr *) &clientAddress, lenClient);   // affectation du nom a la socket
    if (bindReturn < 0) {
        perror("bind()");
        close (socketClient);
        exit(EXIT_FAILURE);
    }

    /* obtention de l'adresse a partir du nom */
    struct hostent *address = gethostbyname(argv[1]);
    if (address == NULL) {
        fprintf(stderr, "invalid address\n");
        exit(EXIT_FAILURE);
    }


    /* creation et configuration de la sockaddr_in du serveur */
    struct sockaddr_in serverAddress;
    memcpy(&serverAddress.sin_addr, *(address->h_addr_list), (size_t) address->h_length);
    memset(serverAddress.sin_zero, 0, sizeof(serverAddress.sin_zero));
    serverAddress.sin_port = htons((uint16_t) atoi(argv[2]));
    serverAddress.sin_family = AF_INET;
    socklen_t lenServer = sizeof(serverAddress);


    /*
     * envoi du message au serveur
     */
    nbChar = sendto(socketClient, message, sizeMessage, 0, (struct sockaddr *) &serverAddress, lenServer);
    if (nbChar != sizeMessage) {
        fprintf(stderr, "sendto()\n");
        close (socketClient);
        exit(EXIT_FAILURE);
    }



    /*
     * reception de la reponse du serveur dans answer
     */
    size_t sizeAnswer = (sizeMessage+8); // calcul de la taille du message retour | 8->"Bonjour "
    char answer[sizeAnswer];

    nbChar = recvfrom(socketClient, answer, sizeAnswer, 0, (struct sockaddr *) &serverAddress, &lenClient);
    if (nbChar != sizeAnswer) {
        fprintf(stderr, "recvfrom() : invalid size\n");
        close(socketClient);
        exit(EXIT_FAILURE);
    }

    /* affichage du message */
    fprintf(stdout, "%s\n", answer);

    close(socketClient);

    return 0;
}