//-------------------------------------------------------
// File: connection.h    Author(s): Simon Désaulniers
//                                  Frédéric Hamelin
// Date: 2013-11-06
//
// © Copyright 2013 Simon Désaulniers, 
//                  Frédéric Hamelin. All Rights Reserved.
//-------------------------------------------------------
// Fichier d'entête du code de gestion des connexions.
//-------------------------------------------------------

#ifndef CONNECTIONS_5VJMUPC6

#define CONNECTIONS_5VJMUPC6

#include <common.h>
#include <client-server.h>

#define DEFAULT_CHANNEL_ID "lobby"

sem_t *cons_sem;
//----------------------------------
//----------------------------------
// Structure d'une connexion dans 
// la liste chaînée
// (doublement chaînée)
//----------------------------------
//----------------------------------
typedef struct _connection {
    char id[32];
    char channel_id[32];
    int sockfd;
    struct sockaddr_in addr;
    int tid;
    sem_t *sem;
    int flagged_deleted;
    struct _connection *next;
    struct _connection *prev;
} connection;

//-------------------------------------------------
// Fonctions associés à la gestion des channels. 
//-------------------------------------------------

//-----------------------------------------------
// wait_for_connection()
// valeurs de retour:
//  0: succès;
//  -1: échec
// Attends après le sémaphore d'une connexion 
// donnée en s'assurant que la connexion n'ait 
// pas été supprimée entre temps.
int wait_for_connection(connection*);

//-----------------
// add_connection()
// valeurs de retour:
//  -2: impossible d'allouer plus de mémoire
//  -1: échec
//  0: succès
// Ajoute une connexion à la liste de connexion en 
// spécifiant les informations suivantes:
//  - username;
//  - channel_id;
//  - ip_address;
//  - client_port.
connection* add_connection(char[], int, struct sockaddr_in, int);

//----------------------
// delete_connection()
// valeurs de retour:
//  -1: échec (la connexion ne peut être trouvée)
//  0: succès
// Supprimme la connexion de la liste de connexions
// en fournissant l'id de la connexion.
int remove_connection(char id[]);

//-------------------------
// find_user()
// valeurs de retour:
//  -1: échec (aucun utilisateur n'a ce id)
//  0: succès
// Trouve l'utilisateur selon son id. Le pointeur
// se verra attribuer l'adresse du noeud trouvé. 
// S'il y a échec, le pointeur se verra attribuer
// NULL.
int find_connection(char[], connection**);

#endif /* end of include guard: CONNECTIONS_5VJMUPC6 */
