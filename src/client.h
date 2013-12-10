#include <client-server.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <common.h>
#include <curses.h>

#define TRY_HELP "essayez inf1010tp2-client --help"
#define HELP "inf1010tp2-client -- client de chat\n"\
             "SYNOPSYS\n"\
                "\tinf1010tp2-client [OPTIONS]\n"\
             "OPTIONS\n"\
                "\t-h|--help\n"\
                    "\t\tAffiche ce texte d'aide.\n"
/*
 * Fonction servant à créer une fenêtre curses
 *
 */
WINDOW *create_newwin(int, int, int, int);

/*
 * Fonction servant à détruire une fenêtre curses 
 *
 */
void destroy_win(WINDOW *);

/*
 * Fonction servant à entrer une commande dans la fenêtre d'input
 *
 */
void inputCommand();

/*
 * Fonction servant à aller chercher un mot dans la chaîne entrée par l'utilisateur
 *
 */
int getWord(char *, char **, int, int); 

/*
 * Fonction servant à créer un socket TCP vers le serveur et à envoyer un paquet de connexion
 *
 */
void connectToServer(char*);

/*
 * Fonction servant à ajouter du texte dans la fenêtre d'affichage
 *
 */
void addText(char*);

/*
 * Fonction servant à ajouter une ligne dans la fenêtre d'affichage
 *
 */
void addLine(char*);

/*
 * Fonction servant à envoyer une commande au serveur
 *
 */
int sendCmdToServer(char*,int);

/*
 * Fonction servant à envoyer un message au serveur
 *
 */
int sendMsgToServer(char*);

/*
 * Fonction servant à envoyer un paquet au serveur
 *
 */
int sendPktToServer(client_packet);

/*
 * Fonction servant à obtenir les options
 *
 */
int get_options(int,char*[]);

/*
 * Fonction servant à recevoir les paquets du serveur
 *
 */
void* listenToServer(void*);
