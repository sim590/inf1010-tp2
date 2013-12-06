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
#define HELP "inf1010tp2-server -- serveur de chat\n\
SYNOPSYS\n\
\tinf1010tp2-server [OPTIONS]\n\
OPTIONS\n\
\t-h|--help\n\
\t\tAffiche ce texte d'aide.\n\
\t-p|--port port\n\
\t\tSpécifie le port sur lequel le serveur devra écouter."

typedef struct _hcc_args {
    int cli_socket;
    struct sockaddr_in cli_addr;
} hcc_args;

int init_socket();
void receive_connections();
void* handle_client_communication(void*);
void close_connection(int, hcc_args*);
int recv_cli_packet(int, client_packet*);
int send_srv_packet(int,server_packet*);
int get_options(int,char*[]);
#endif /* end of include guard: SERVER_DTVL1IT7 */
