//-------------------------------------------------------
// File: server.c    Author(s): Simon Désaulniers
//                              Frédéric Hamelin
// Date: 2013-10-22
//
// © Copyright 2013 Simon Désaulniers, 
//                  Frédéric Hamelin. All Rights Reserved.
//-------------------------------------------------------
// Code côté serveur.
//-------------------------------------------------------
#include <server.h>

int reception_socket_fd, server_port = DEFAULT_SERVER_PORT;
struct sockaddr_in srv_addr;

int init_socket()
{
    // Création du scoket de réception
    reception_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (reception_socket_fd) {
        perror("Erreur lors de l'obtention du socket de réception.");
        return -1;
    }

    // structure d'addresse à laquelle se bind()
    srv_addr = (struct sockaddr_in) {
        AF_INET, // socket sur internet
        htons(server_port), // le port d'écoute 
        (struct in_addr) {INADDR_ANY}, // l'adresse de la machine 
        0 // aucune idée.. 
    };

    if (bind(reception_socket_fd, (struct sockaddr *) &srv_addr, sizeof(srv_addr))) {
        perror("Erreur lors de la réservation du socket. Socket déjà utilisé?");
        return -1;
    }
}
void* listen_clients()
{
    int clienlen, n;

    // TODO: écouter le socket pour des connexions
}

int get_options(int argc, char *argv[])
{
    int opt;
    
    static const struct option longopts[]= {
        {"help" , no_argument       , 0 , 'h' },
        {"port" , required_argument , 0 , 'p' },
        {0      , 0                 , 0 ,  0  }
    };

    while ((opt = getopt_long(argc, argv, "hp:", longopts, NULL)) != -1) {
        switch (opt) {
            case 'h':
                printf("%s\n", HELP);
                exit(EXIT_SUCCESS);
            case 'p':
                if (optarg)
                    server_port = atoi(optarg);
                break;
            default:
                fprintf(stderr, "%s\n", TRY_HELP);
                return -1;
        }
    }
}

int main(int argc, char *argv[])
{
    
    if(get_options(argc, argv))
        exit(EXIT_FAILURE);

    if (init_socket()) {
        fprintf(stderr, "Erreur lors de l'initialisation du serveur...\n");
        exit(EXIT_FAILURE);
    }

    listen_clients();
    return 0;
}
