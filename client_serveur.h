//
// Created by guillaume on 22/12/16.
//

#ifndef PROJETRESEAU_CLIENT_SERVEUR_H_H
#define PROJETRESEAU_CLIENT_SERVEUR_H_H
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

/* CrÃ©ation d un serveur */
extern int creer_serveur_tcp ( int , int, int );
/* Attente d un client */
extern int attendre_client_tcp ( int , int );
/* Dans un serveur retourne l adresse du client connectÃ© */
extern char *  adresse_client ( void );
/* CrÃ©ation d un client */
extern int creer_client_tcp ( char * , int , int );
/* Retourne l adresse en format rÃ©seau de l adresse en notation pointÃ©e */
extern int adresse (char *, int );
/* CrÃ©ation d un serveur */
extern int creer_serveur_udp ( int , int );
/* CrÃ©ation d un client */
extern int creer_client_udp ( char * , int, struct sockaddr_in *,
                              struct sockaddr_in *, int);
/* Lit ou Ã©crit n caractÃ¨res sur un descripteur ne s arrÃªte qu'en cas d'erreur */
extern int ecrire_n ( int, char * , int);
extern int lire_n   ( int, char *, int);
#endif //PROJETRESEAU_CLIENT_SERVEUR_H_H
