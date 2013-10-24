//-------------------------------------------------------
// File: client-server.h    Author(s): Simon DÉSAULNIERS
// Date: 2013-10-22
//-------------------------------------------------------
// Ficheir d'entête pour le client et le serveur.
//-------------------------------------------------------
#ifndef CLIENT-SERVER_NN925AM4
#define CLIENT-SERVER_NN925AM4
#define SERVER_PORT 1346;

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


// --------------------------------------------------------------------------
// DÉFINITION DE LA LISTE DES COMMANDES
//
// - msg:
//  description: Envoie un message à l'utilisateur spécifié.
//  synopsis: /msg <nom_utilisateur> <message>
//      - nom_utilisateur: Le nom d'utilisateur de la personne à qui envoyer le
//      message.
//      - message: Le message à envoyer.
//
// - names
//  synopsis: /names
//  description: Requête de la liste des clients connectés dans le canal.
//
// - list
//  description: Requête de la liste de canaux disponibles.
//  synopsis: /list
//
// - connect
// description: Connecte le client au serveur spécifié.
// synopsis: /connect <nome_hôte>
//      - nome_hôte: Le nom d'ĥôte du serveur auquel se connecter.
//
// - join
//  description: Change le canal dans lequel le client clavarde.
//  synopsis: /join <nom_canal>
//      - nom_canal: Le nom du canal auquel se joindre.
//
// - disconnect
//  description: Déconnecte le client du serveur.
//  synopsis: /disconnect
//
// - quit
//  description: Ferme le programme.
//  synopsis: /quit
//
// --------------------------------------------------------------------------


// --------------------------------------------------
// Structure d'une commande pour envoyer au serveur.
//
// ex: /msg fred blablabla
//  on aurait:
//  - cmd = "msg"
//  - arg[0] = "fred"
//  - arg[3] = "blablabla" 
//      >>le dernier est tout le temps l'argument 
//        principale.
// --------------------------------------------------
typedef struct _client_cmd {
    unsigned int type;
    char cmd[16];
    char arg[4][256];
} client_cmd;

// -------------------------------
// Structure d'un simple message 
// texte par le client
// -------------------------------
typedef struct _client_message {
    unsigned int type;
    char message[256];
} client_text_message;

typedef union {
    unsigned int type;
    client_cmd cmd;
    client_text_message;
} client_packet;

#endif /* end of include guard: CLIENT-SERVER_NN925AM4 */
