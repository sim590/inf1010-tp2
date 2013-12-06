//-------------------------------------------------------
// File: client-server.h    Author(s): Simon DÉSAULNIERS
//                                     Frédéric Hamelin
// Date: 2013-10-22
//-------------------------------------------------------
// Ficheir d'entête pour le client et le serveur.
//-------------------------------------------------------
#ifndef CLIENT_SERVER_NN925AM4
#define CLIENT_SERVER_NN925AM4
#define SERVER_PORT 1346;

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
// threads
#include <semaphore.h>
#include <pthread.h>


#define DEFAULT_SERVER_PORT 28000
#define BIG_MESSAGE_SIZE 1024

// --------------------------------------------------------------------------
// DÉFINITION DE LA LISTE DES COMMANDES
//
// - msg:
//  description: Envoie un message à l'utilisateur spécifié.
//  synopsis: /msg <nom_utilisateur> <message>
//      - nom_utilisateur: Le nom d'utilisateur de la personne à qui envoyer le
//                         message. Si le nom d'utilisateur est "-", le message
//                         est envoyé à tous les utilisateurs connectés au
//                         serveur.
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
//  - argc = 2
//  - args[0] = "msg"
//  - args[1] = "fred"
//  - main_arg = "blablabla" 
// --------------------------------------------------
typedef struct _client_cmd {
    unsigned int type; // = 2 
    int argc;
    char args[5][32];
    char main_arg[256];
} client_cmd;

// --------------------------------------------------------------
// Structure contenant les informations nécessaires au serveur
// lors de la connexion du client.
// --------------------------------------------------------------
typedef struct _connection_info {
    unsigned int type; // = 1 
    char id[32];
} connection_info;

typedef union {
    unsigned int type; /* in {-1,0,1,2}
                        * 0: message texte.
                        * 1: connexion
                        * -1: déconnexion
                        */
    client_cmd cmd;
    connection_info con_info;
    char message[256];
} client_packet;

typedef union {
    unsigned int type;  /* in {-1,0,1}.
                         * -1: fail
                         *  0: succès de connexion ou de déconnexion
                         *  1: message
                         *  2: big_message
                         */
    char message[256];
    char big_message[BIG_MESSAGE_SIZE];
} server_packet;

#endif /* end of include guard: CLIENT_SERVER_NN925AM4 */
