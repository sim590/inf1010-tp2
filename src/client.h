#include <client-server.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curses.h>

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
