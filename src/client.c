#include "client.h"

char* server_ip;
int des;
struct sockaddr_in server;
WINDOW * mainwin;

int main(void)
{

	if ( (mainwin = initscr()) == NULL ) {
	fprintf(stderr, "Error initialising ncurses.\n");
	exit(EXIT_FAILURE);
    }

    inputCommand();

    mvaddstr(13, 33, "Hello, world!");
    refresh();
    sleep(3);


    /*  Clean up after ourselves  */

    delwin(mainwin);
    endwin();
    refresh();

    return EXIT_SUCCESS;

	//if (des = socket(AF_INET, SOCK_STREAM, 0) < 0) {
	//exit(1);
	//}

	//server.sin_addr.saddr = inet_addr("")	
}

void inputCommand()
{
	
}

//int connectToServer()
//{

//}

//int sendToServer()
//{

//}

