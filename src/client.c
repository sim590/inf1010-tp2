#include "client.h"
#include <curses.h>

char* server_ip;
int des;
struct sockaddr_in server;
WINDOW *displayWin;
WINDOW *inputWin;

int main(void)
{
  
    initscr();

    displayWin = create_newwin(LINES-3,COLS-1,0,0);
    inputWin = create_newwin(3,COLS-1,LINES-3,0);
    wrefresh(displayWin);
    wrefresh(inputWin);


    inputCommand();

    mvwaddstr(displayWin, 1,1, "Hello, world!");
    mvwaddstr(inputWin,1,1, "input");
    sleep(3);

    /*  Clean up after ourselves  */

    destroy_win(displayWin);
    destroy_win(inputWin);
    endwin();
    refresh();

    return 0;

	//if (des = socket(AF_INET, SOCK_STREAM, 0) < 0) {
	//exit(1);
	//}

	//server.sin_addr.saddr = inet_addr("")	
}

void inputCommand()
{
    char * str = malloc(sizeof(char) * 255);

    mvwaddstr(inputWin,1,1,"");
    wgetstr(inputWin, str);
    
    char * command;
    getNextWord(str,&command);
    mvwaddstr(displayWin, 7,7,command);
    wrefresh(displayWin);

    if (!strcmp(command, "connect")) {
        connectToServer();
    }
    else if (!strcmp(command, "quit")) {
        // quit();
    }
    else {
       // sendToServer(); 
    }

    free(str);
    free(command);
}

int getNextWord(char *str, char **nextWord)
{

    char * token = NULL;
    
    while (*str == ' ') {
        str++;
    }

    char * inputCopy = strdup(str);

    token = strtok(inputCopy, " ");

    if (!token){
        *nextWord = NULL;
        return -1;
    }

    *nextWord = malloc(sizeof(char) * strlen(token));
    strcpy(*nextWord,token);

    str += strlen(token);

    free(inputCopy);

    return 0;
}

int connectToServer()
{ 
    mvwaddstr(displayWin, 1,1,"Connecting to server...");
    wrefresh(displayWin);

    return 0;
}

//int sendToServer()
//{

//}

WINDOW *create_newwin(int height, int width, int starty, int startx)
{  
    WINDOW *local_win;

    local_win = newwin(height, width, starty, startx);
    box(local_win, 0 , 0);     
    wrefresh(local_win);
   
    return local_win;
}

void destroy_win(WINDOW *local_win)
{   
    wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    wrefresh(local_win);
    delwin(local_win);
}
