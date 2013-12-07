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
WINDOW *create_newwin(int, int, int, int);

void destroy_win(WINDOW *);

void inputCommand();

int getWord(char *, char **, int, int); 

void connectToServer(char*);

void addText(char*);

void addLine(char*);

int sendCmdToServer(char*,int);

int sendMsgToServer(char*);

int sendPktToServer(client_packet);

int get_options(int,char*[]);

void* listenToServer(void*);
