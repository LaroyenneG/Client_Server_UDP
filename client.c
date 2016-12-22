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
#include <arpa/inet.h>

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


    //concatenation
    for(int i=start; i<argc; i++){
        strcat(message, argv[i]);
        if(i+1<argc){
            strcat(message, " ");
        }
    }


    //envoi du message

    struct sockaddr_in serveur, client;
    socklen_t lserveur=(socklen_t) sizeof(serveur);

    //socketClient = creer_client_udp( argv[1], atoi(argv[2]), &serveur, &client, 1);

    int socketClient=socket(AF_INET, SOCK_DGRAM, 0);

    if(socketClient<0){
        perror("socket()");
        exit(EXIT_FAILURE);
    }

    bzero( &client , sizeof(client) );
    client.sin_family = AF_INET;
    client.sin_addr.s_addr = htonl(INADDR_ANY);
    client.sin_port = htons ( 0 );

    int bindReturn = bind (socketClient, (const struct sockaddr *) &client, sizeof (client) );

    if ( bindReturn < 0 ){
        perror("bind()");
        close (socketClient);
        exit(EXIT_FAILURE);
    }

    bzero( &serveur, sizeof(serveur) );
    serveur.sin_family = AF_INET;
    serveur.sin_addr.s_addr = inet_addr(argv[1]);
    serveur.sin_port = htons ((uint16_t) atoi(argv[2]));


    ssize_t n=sendto(socketClient, (void *) message, (size_t) sizeMessage, 0, (struct sockaddr *) &serveur, (socklen_t) sizeof(serveur));

    if ( n != (ssize_t)sizeMessage) {
        perror("sendto()");
        close (socketClient);
        exit(EXIT_FAILURE);
    }
    //reception du message

    int sizeAnswer= (int) (strlen(message) + 1 + 8); // 8->"Bonjour "
    char answer[sizeAnswer];

    n=recvfrom(socketClient, (void *) answer, (size_t) (sizeAnswer), 0, (struct sockaddr *) &serveur, &lserveur);


    if ( n != sizeAnswer){
        perror("recvfrom()");
        close(socketClient);
        return EXIT_FAILURE;
    }


    printf("Message recu du serveur : %s\n",answer);
    close(socketClient);

    return 0;
}