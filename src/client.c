#include <client.h>

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
    char *str;

    str = malloc(sizeof(char));

    mvwaddstr(inputWin,1,1,"");
    wgetstr(inputWin, str);

    int *position;
    *position = 0; 
    
    char *command = malloc(sizeof(char));
    command = getNextWord(str, position);
    mvwaddstr(displayWin, 1,1,command);
    wrefresh(displayWin);

    if (strcmp(command, "connect") == 0) {
       // connectToServer();
    }
    else if (strcmp(command, "quit") == 0) {
        // quit();
    }
    else {
       // sendToServer(); 
    }

    mvwaddstr(displayWin, 1,1,command);
    wrefresh(displayWin);


    free(str);
    free(command);
}

char* getNextWord(char *str,int * position)
{
    char* word;
    int initpos = *position;
    char strTab[sizeof(*str)];

    strcpy(strTab, str);

    while (*position < sizeof(*str) && (sizeof(*word) == 0) || strTab[*position] != ' ') {
        *position++;
    }
    word = malloc(sizeof(char)*(*position-initpos)+1);
    strncpy(word, str+initpos, *position-initpos+1);
    return word;
}

//int connectToServer()
//{

//}

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
