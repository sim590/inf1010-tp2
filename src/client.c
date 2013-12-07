#include <client.h>

char* server_ip;
int server_port = DEFAULT_SERVER_PORT;
int des;
struct sockaddr_in s_in;
WINDOW *displayWin;
WINDOW *inputWin;
int sock;

int get_options(int argc, char *argv[])
{
    int opt;
    
    static const struct option longopts[] = {
        {"help" , no_argument       , 0 , 'h' },
        {0      , 0                 , 0 ,  0  }
    };

    while ((opt = getopt_long(argc, argv, "h:", longopts, NULL)) != -1) {
        switch (opt) {
            case 'h':
                printf("%s\n", HELP);
                exit(EXIT_SUCCESS);
            default:
                fprintf(stderr, "%s\n", TRY_HELP);
                return -1;
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if(get_options(argc, argv))
        exit(EXIT_FAILURE);
  
    initscr();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);

    displayWin = create_newwin(LINES-1,COLS-1,0,0);
    inputWin = create_newwin(1,COLS-1,LINES-1,0);

    wattron(displayWin, COLOR_PAIR(1));
    wattron(inputWin, COLOR_PAIR(2));

    inputCommand();


    /*  Clean up after ourselves  */

    destroy_win(displayWin);
    destroy_win(inputWin);
    endwin();

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

    if (*str != '/') {
       sendCmdToServer(str, 0);
    }
    else {
    
        char * command;
        getWord(str,&command,1,0);

        if (!strcmp(command, "connect")) {
            connectToServer(str);
        }
        else if (!strcmp(command, "quit")) {
            return;
        }
        else if (!strcmp(command, "msg")) {
            sendCmdToServer(str,2);
        }
        else {
            sendCmdToServer(str,1); 
        }

        free(command);
    }

    free(str);

    inputCommand();
}

int getWord(char * str,char **nextWord, int position, int main)
{
    int cur_pos = 0;
    char * token = NULL;

    if (*str == '/') {
        str++;
    }
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
    
    char * port;
    getWord(str,&port,3,0);

    s_in.sin_family = AF_INET;
    s_in.sin_port = htons(atoi(port));
    //TODO: Utiliser server_port si aucun port spécifié
    //s_in.sin_port = htons(server_port);
    inet_aton(ip_address, &s_in.sin_addr);

    if (connect(sock,(struct sockaddr*)&s_in, sizeof(s_in)) < 0) {
        //TODO: gérer ça
        exit(EXIT_FAILURE);
    }

    client_packet pkt;

    pkt.type = 1;
    
    char * user_id;
    getWord(str,&user_id,4,0);

    strcpy(pkt.con_info.id, user_id);
}

int sendMsgToServer(char * str)
{
    client_packet pkt;

    pkt.type = 0;
    pkt.msg.type = 0;
    strcpy(pkt.msg.message,str);

    sendPktToServer(pkt);

    return 0;
}

int sendCmdToServer(char * str, int argc)
{
    int count = 0;
    client_packet pkt;

    pkt.cmd.argc = argc;
    pkt.type = 2;
    pkt.cmd.type = 2;

    char * arg;
    while (count < argc) {
        getWord(str,&arg,count + 1, 0);
        strcpy(pkt.cmd.args[count], arg);
        count++;
    }

    getWord(str,&arg,count + 1, 1);
    
    strcpy(pkt.cmd.main_arg,arg);

    sendPktToServer(pkt);

    return 0;
}

int sendPktToServer(client_packet pkt)
{
    send(sock,(void*)&pkt, sizeof(pkt),0);

    return 0;
}

WINDOW *create_newwin(int height, int width, int starty, int startx)
{  
    WINDOW *local_win;

    local_win = newwin(height, width, starty, startx);
   
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
