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

int main(int argc, char** argv) {

    if(argc!=2) {
        fprintf(stderr,"Usage : serveur <port>\n");
        exit(EXIT_FAILURE);
    }

    /*
     * variables globales
     */
    const size_t sizeMessage = 65536;      // taille maximale du message
    const char bonjour[] = "Bonjour ";    // pour le message retour du serveur
    ssize_t nbChar;                      // pour le nombre d'octets lus et envoyes


    /*
     * creation et configuration du serveur
     */

    /* creation de la socket du serveur */
    int serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket < 0) {
        perror("socket()");
        exit(EXIT_FAILURE);
    }

    /* creation et configuration de la sockaddr_in du serveur */
    struct sockaddr_in serverAddress;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    memset(serverAddress.sin_zero, 0, sizeof(serverAddress.sin_zero));
    serverAddress.sin_port = htons((uint16_t) atoi(argv[1]));
    serverAddress.sin_family = AF_INET;
    socklen_t lenServer = sizeof(serverAddress);

    int bindReturn = bind(serverSocket, (struct sockaddr*) &serverAddress, lenServer);  // affectation du nom a la socket
    if (bindReturn < 0) {
        perror("bind()");
        close (serverSocket);
        exit(EXIT_FAILURE);
    }

    /*
     * gestion des clients
     */
    while(1){

        /*
         * reception des clients
         */

        /* creation de la sockaddr_in pour le client */
        struct sockaddr_in clientAddress;
        socklen_t lenClient = sizeof(clientAddress);

        /* reception du message et configuration de la sockaddr_in suivant le client */
        char message[sizeMessage+1];
        message[sizeMessage]='\0';      // marqueur de fin de chaine a la fin de la taille maximale du message par securite (pour strcat) dans le cas ou le client n'enverrait pas le marqueur de fin de chaine
        nbChar = recvfrom(serverSocket, message, sizeMessage, 0, (struct sockaddr*) &clientAddress, &lenClient);
        if (nbChar <= 0) {
            fprintf(stderr, "No byte received\n");
            close(serverSocket);
            exit(EXIT_FAILURE);
        }

        /* recuperation du domaine dans hostname si il existe */
        char domaine[NI_MAXHOST];
        int nameInfoReturn=getnameinfo((const struct sockaddr*) &clientAddress, lenClient, domaine, sizeof(domaine), NULL, 0, NI_NAMEREQD);

        /* affichage des informations du client sur la sortie standard
           en fonction de la reussite de la recuperation du domaine */
        if (nameInfoReturn < 0) {
            fprintf(stdout, "CLIENT: %s:%d\n", inet_ntoa(clientAddress.sin_addr), clientAddress.sin_port);
        } else {
            fprintf(stdout, "CLIENT: %s:%d (%s)\n", inet_ntoa(clientAddress.sin_addr), clientAddress.sin_port, domaine);
        }

        /* concatenation du message retour dans answer */
        char answer[strlen(message)+strlen(bonjour)+1];
        answer[0]='\0';                                 // transformation en "string" vide pour strcat()
        strcat(answer, bonjour);
        strcat(answer,message);

        /* envoi de la reponse au client */
        nbChar = sendto(serverSocket, answer, strlen(answer) + 1, 0, (struct sockaddr *) &clientAddress, lenClient);
        if (nbChar != strlen(answer)+1) {
            fprintf(stderr, "sendto() : invalid size\n");
            close(serverSocket);
            exit(EXIT_FAILURE);
        }
    }

    close(serverSocket);

    return 0;
}
