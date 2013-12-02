//#include <client-server.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <curses.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

WINDOW *create_newwin(int height, int width, int starty, int startx);

void destroy_win(WINDOW *local_win);

void inputCommand();

int getNextWord(char * str, char ** nextWord); 

void addText(char*);

void addLine(char*);
