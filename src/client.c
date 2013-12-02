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
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);

    displayWin = create_newwin(LINES-2,COLS-1,0,0);
    inputWin = create_newwin(1,COLS-1,LINES-1,0);

    wattron(displayWin, COLOR_PAIR(1));
    wattron(inputWin, COLOR_PAIR(2));
    wrefresh(displayWin);
    wrefresh(inputWin);

    inputCommand();

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

    wmove(inputWin,0,0);
    winsertln(inputWin);
    waddstr(inputWin,"$");
    wrefresh(inputWin);
    wgetstr(inputWin, str);
    
    char * command;
    getNextWord(str,&command);

    addText(str);

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

    inputCommand();
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
    wrefresh(local_win);
   
    return local_win;
}

void destroy_win(WINDOW *local_win)
{   
    wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    wrefresh(local_win);
    delwin(local_win);
}

void addText(char * text)
{
    char * text_copy = strdup(text);

    char * token = NULL;
    
    do {
        token = strtok(text_copy, "\n");

        if (token) {
            text_copy += strlen(token);
            addLine(token);
        }
    } while (token);
}

void addLine(char * line)
{
    wmove(displayWin,1,1);
    wdeleteln(displayWin);
    wmove(displayWin,LINES-3,1);
    winsertln(displayWin);
    waddstr(displayWin,line);
    wrefresh(displayWin);

}
