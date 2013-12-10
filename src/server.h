//-------------------------------------------------------
// File: server.h    Author(s): Simon Désaulniers
//                              Frédéric Hamelin
// Date: 2013-10-22
//
// © Copyright 2013 Simon Désaulniers, 
//                  Frédéric Hamelin. All Rights Reserved.
//-------------------------------------------------------
// Entête du serveur.
//-------------------------------------------------------
#ifndef SERVER_DTVL1IT7

#define SERVER_DTVL1IT7

//communication client-serveur
#include <client-server.h>
//utilités serveur
#include <channels.h>
#include <connections.h>
// common
#include <common.h>

#define TRY_HELP "essayez inf1010tp2-server --help"
#define HELP "inf1010tp2-server -- serveur de chat\n"\
             "SYNOPSYS\n"\
                "\tinf1010tp2-server [OPTIONS]\n"\
             "OPTIONS\n"\
                "\t-h|--help\n"\
                    "\t\tAffiche ce texte d'aide.\n"\
                "\t-p|--port port\n"\
                    "\t\tSpécifie le port sur lequel le serveur devra écouter."

typedef struct _hcc_args {
    int cli_socket;
    struct sockaddr_in cli_addr;
} hcc_args;

//-------------------------------------------------
// init_socket()
// valeur de retour:
//  0: succès
//  -1: échec
// réserve et initialise le socket de rendez-vous
int init_socket();

//---------------------------------------------------
// receive_connections()
//
// Écoute les demande de connexions des clients et 
// démarre les « threads » de connexion.
void receive_connections();

//-----------------------------------------------------
// handle_client_communication()
//
// Gère la connexion avec le client. Reçoit toute les
// requête de celui-ci. Cette fonction est « threadé ».
void* handle_client_communication(void*);

//---------------------------------------------------
// close_connection()
//
// Fait la fermeture de la connexion (fermeture de 
// socket, etc.)
void close_connection(connection*, hcc_args*);

//------------------------------------------------
// recv_cli_packet()
// valeur de retour:
//  0: succès
//  -1: Impossible d'écouter le socket après un 
//      certain nombre d'essais.
// Fait un certain nombre de tentatives d'écoute 
// du client. Si c'est un succès, la structure 
// pointée par le pointeur passé en paramètre 
// contiendra les informations envoyés par le client.
int recv_cli_packet(connection*, client_packet*);

//------------------------------------------------
// send_srv_packet()
// valeur de retour:
// 0: succès
// -1: échec après quelques tentatives.
//
// Fait un certain nombre de tentatives pour
// écrire au client. Ce qui sera écrit est le
// contenu pointé par le pointeur server_packet
// passé en paramètre.
int send_srv_packet(connection*,server_packet*);

//-----------------------------------------------
// get_options()
// valeur de retour:
//  0: succès
//  -1: échec (option invalide)
// Récupère les options à la ligne de commande.
int get_options(int,char*[]);
#endif /* end of include guard: SERVER_DTVL1IT7 */
