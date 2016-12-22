//
// Created by guillaume on 16/12/16.
//

#include <stdlib.h>
#include <stdio.h>
#include <tkPort.h>
#include <netinet/in.h>
#include <string.h>
#include <hdf5.h>
#include <hdf5_hl.h>
#include "client_serveur.h"

int main(int argc, char* argv[]) {

    if(argc<4){
        perror("Usage : client <address> <port> <message>");
        exit(-1);
    }



    int start=3;

    //calcul de la taille du message
    int sizeMessage=0;
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

    char message[sizeMessage];
    memset(message, '\0', sizeof(message));

    printf("la taille est de %d \n",sizeMessage);

    //concatenation
    for(int i=start; i<argc; i++){
        strcat(message, argv[i]);
        if(i+1<argc){
            strcat(message, " ");
        }
    }

    //affichage du message
    printf("%s\n",message);


    //envoi du message
    int socket;
    struct sockaddr_in serveur, client;
    socklen_t lserveur=(socklen_t) sizeof(serveur);

    socket = creer_client_udp( argv[1], atoi(argv[2]), &serveur, &client, 1);

    if ( socket < 0 ) {
        perror("socket creation");
        exit(-1);
    }

    ssize_t n=sendto(socket, (void *) message, (size_t) sizeMessage, 0, (struct sockaddr *) &serveur, (socklen_t) sizeof(serveur));

    if ( n != (ssize_t)sizeMessage) {
        perror("sendto()");
        close (socket);
        exit(-1);
    }
    //reception du message

    int sizeAnswer= (int) (strlen(message) + 1 + 8); // 8->"Bonjour "
    char answer[sizeAnswer];

    n=recvfrom(socket, (void *) answer, (size_t) (sizeAnswer), 0, (struct sockaddr *) &serveur, &lserveur);


    if ( n != sizeAnswer){
        perror("recvfrom()");
        close(socket);
        return EXIT_FAILURE;
    }


    printf("Message recu du serveur : %s\n",answer);
    close(socket);

    return 0;
}