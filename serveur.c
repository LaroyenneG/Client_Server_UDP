//
// Created by guillaume on 16/12/16.
//


#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <tkPort.h>
#include "client_serveur.h"


int main(int argc, char** argv) {
    int sizeMessage=256;

    if(argc!=2){
        perror("Usage : serveur <port>");
        exit(-1);
    }

    int sockfd;

    /* sockaddr_in structure pour le client */
    struct sockaddr_in client;
    int lg_client=sizeof( client );


    sockfd = creer_serveur_udp( atoi(argv[1]), 1 );

    if ( sockfd < 0 ) {
        fprintf(stderr,"Serveur: echec de la fonction  creer_serveur_udp\n");
        exit(EXIT_FAILURE);
    }

    while(1){

        char message[sizeMessage];

        ssize_t n;

        n = recvfrom (sockfd, message, (size_t) sizeMessage, 0 , (struct sockaddr *)&client , (socklen_t *) &lg_client);

        if ( n <= 0 ) {
            perror("No byte received");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        printf("Message recu du client:%s\n",message);


        char domaine[NI_MAXHOST];

        int getnameReturn=getnameinfo((const struct sockaddr *) &client, (socklen_t) lg_client, domaine, sizeof(domaine), NULL, 0, NI_NAMEREQD);

        if (getnameReturn<0){
            printf("CLIENT: %s:%d\n",inet_ntoa(client.sin_addr), client.sin_port);
        } else{
            printf("CLIENT: %s:%d (%s)\n",inet_ntoa(client.sin_addr), client.sin_port, domaine);
        }

        // Envoi du message au client
        char bonjour[]="Bonjour ";
        char answer[sizeMessage+strlen(bonjour)];
        memset(answer, '\0', sizeof(answer));
        strcat(answer, bonjour);
        strcat(answer,message);

        printf("Nouveau message:%s\n",answer);


        n = sendto (sockfd, answer , strlen(answer) + 1 , 0, (struct sockaddr *)&client , (socklen_t) lg_client);

        if ( n != strlen(answer)+1 ) {
            perror("sendto()");
            close ( sockfd);
            exit (EXIT_FAILURE);
        }
    }

    return 0;
}
