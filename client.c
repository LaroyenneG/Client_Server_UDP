//
// Created by guillaume on 16/12/16.
//

#include <stdlib.h>
#include <stdio.h>
#include <tkPort.h>
#include "string.h"

#include "./fonctions_creation_client_serveur/lib/client_serveur.h"

int main(int argc, char* argv[]) {

    if(argc<4){
        perror("Usage : client <address> <port> <message>");
        exit(-1);
    }

    char message[256];
    memset(message, '\0', sizeof(message));

    int start=3;

    //calcul de la taille du message
    int size=0;
    for(int a=start; a<argc; a++){
        size+=strlen(argv[a]);
        if(a+1<argc){
            size++;
        }
    }
    if(size>256){
        perror("over size");
        exit(-1);
    }

    printf("la taille est de %d \n",size);

    //concatenation
    for(int i=start; i<argc; i++){
        strcat(message, argv[i]);
        if(i+1<argc){
            strcat(message, " ");
        }
    }

    //affichage du message
    printf("%s\n",message);



    return 0;
}