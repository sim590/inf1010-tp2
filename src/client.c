#include "client.h"
#include <curses.h>

char* server_ip;
int des;
struct sockaddr_in s_in;
WINDOW *displayWin;
WINDOW *inputWin;
int sock;

int main(void)
{
  
    initscr();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);

    displayWin = create_newwin(LINES-1,COLS-1,0,0);
    inputWin = create_newwin(1,COLS-1,LINES-1,0);

    wattron(displayWin, COLOR_PAIR(1));
    wattron(inputWin, COLOR_PAIR(2));
    wrefresh(displayWin);
    wrefresh(inputWin);

    inputCommand();


    /*  Clean up after ourselves  */

    destroy_win(displayWin);
    destroy_win(inputWin);
    endwin();
    refresh();

    exit(EXIT_SUCCESS);

}

void inputCommand()
{
    char * str = malloc(sizeof(char) * 255);

    wmove(inputWin,0,0);
    winsertln(inputWin);
    waddstr(inputWin,"$");
    wrefresh(inputWin);
    wgetstr(inputWin, str);

    wattron(displayWin, COLOR_PAIR(3));
    addText(str);
    wattron(displayWin, COLOR_PAIR(1));

    char * command;
    getWord(str,&command,1,0);

    if (!strcmp(command, "connect")) {
        connectToServer(str);
    }
    else if (!strcmp(command, "quit")) {
        return;
    }
    else if (!strcmp(command, "msg")) {
        sendToServer(str,2);
    }
    else {
        sendToServer(str,1); 
    }

    free(str);
    free(command);

    inputCommand();
}

int getWord(char * str,char **nextWord, int position, int main)
{
    int cur_pos = 0;
    char * token = NULL;

    while (cur_pos < position) {
    
        token = NULL;

        while (*str == ' ' || *str == '\0') {
            str++;
        }

        if (position - cur_pos == 1 && main) {
            token = strdup(str); 
        } else {
            token = strtok(str, " ");
        }

        if (!token){
            *nextWord = NULL;
            return -1;
        }
        
        cur_pos++;
        str += strlen(token);
    }
    *nextWord = malloc(sizeof(char) * (strlen(token) + 1));
    strcpy(*nextWord,token);

    //free(inputCopy);

    return 0;
}

void connectToServer(char * str)
{ 
    char * ip_address;
    getWord(str,&ip_address,2,0);

    addText("Connecting to server...");
    addText(ip_address);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        return;
    }

    s_in.sin_family = AF_INET;
    s_in.sin_port = htons(80);
    inet_aton(ip_address, &s_in.sin_addr);
}

int sendToServer(char * str, int argc)
{
    int count = 0;
    struct _client_cmd cmd;

    cmd.argc = argc;
    
    char * arg;
    while (count < argc) {
        getWord(str,&arg,count + 1, 0);
        strcpy(cmd.args[count], arg);
        count++;
    }

    getWord(str,&arg,count + 1, 1);
    
    strcpy(cmd.main_arg,arg);
    return 0;
}

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
    wmove(displayWin,0,0);
    wdeleteln(displayWin);
    wmove(displayWin,LINES-2,0);
    winsertln(displayWin);
    waddstr(displayWin,line);
    wrefresh(displayWin);
}
