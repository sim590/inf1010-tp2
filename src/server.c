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
struct sockaddr_in srv_addr, cli_addr;
extern connection *first_con;

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
        AF_INET,                       // socket sur internet
        htons(server_port),            // le port d'écoute
        (struct in_addr) {INADDR_ANY}, // l'adresse de la machine
        0                              // aucune idée..
    };

    if (bind(reception_socket_fd, (struct sockaddr *) &srv_addr, sizeof(srv_addr))) {
        perror("Erreur lors de la solicitation du socket. Socket déjà utilisé?");
        return -1;
    }
}

void receive_connections()
{
    int clilen = sizeof(cli_addr), sockfd;
    pthread_t tid;

    if (listen(reception_socket_fd, 5)) {
        perror("Erreur lors de la mise en écoute sur le socket.");
        exit(EXIT_FAILURE);
    }

    while (1) {
        sockfd = accept(reception_socket_fd, (struct sockaddr*) &cli_addr, &clilen);

        hcc_args *args = malloc(sizeof(hcc_args));
        *args = (hcc_args) {sockfd, cli_addr};

        
        if (pthread_create(&tid, NULL, handle_client_communication, args))
            fprintf(stderr, "Impossible de créer un thread de connexion pour le client %u.%u.%u.%u...\n", 
                    cli_addr.sin_addr.s_addr>>24,
                    (cli_addr.sin_addr.s_addr<<8)>>24,
                    (cli_addr.sin_addr.s_addr<<16)>>24,
                    (cli_addr.sin_addr.s_addr<<24)>>24
             );
    }
}

void* handle_client_communication(void *argss)
{
    // récupération des arguments
    pthread_t my_tid = pthread_self();
    hcc_args *args = (hcc_args*) argss;

    client_packet cli_packet;
    server_packet srv_packet;
    int sockfd = args->cli_socket, n=0;

    // lecture du premier paquet (réception du id de client)
    if (recv_cli_packet(sockfd, &cli_packet)) {
        close_connection(sockfd, args);
        return NULL;
    }

    // ajout de la connexion avec le client à la liste
    connection *cur, *my_con = add_connection(
                cli_packet.con_info.id, 
                sockfd,
                args->cli_addr, 
                my_tid
            );

    // ne peut ajouter la connexion avec le client à la liste.
    if (!my_con) {
        srv_packet = (server_packet) {.type = -1};
        send(sockfd, (void*)&srv_packet, sizeof(server_packet), 0);
    }
    else {
        // confirmation du succès
        srv_packet = (server_packet) {.type = 0};
        if (send_srv_packet(sockfd, &srv_packet)) {
            close_connection(sockfd, args);
            return NULL;
        }

        // s'occuper des requêtes du clients
        while (1) {

            if (recv_cli_packet(sockfd, &cli_packet)) {
                close_connection(sockfd, args);
                return NULL;
            }

            switch(cli_packet.type) {
                // déconnexion
                case -1:
                    close_connection(sockfd, args);
                    return NULL;
                // envoie d'un message au canal du client
                case 0:
                    cur = first_con;
                    while (cur) {
                        if (!strcmp(cur->channel_id, my_con->channel_id)) {
                            // send sur le socket du client cur.
                            srv_packet.type = 0;
                            strcpy(srv_packet.msg.message, cli_packet.msg.message);
                            send_srv_packet(cur->sockfd, &srv_packet);
                        }
                    }
                    break;
                //TODO:
                // commande format "/cmd arg1 arg2 ..."
                case 2:
                    break;
                // mauvais paquet
                default:
                    break;
            }
        }
    }

    close_connection(sockfd, args);
    return NULL;
}

void close_connection(int sockfd, hcc_args *args)
{
    free(args);
    close(sockfd);
}

//TODO: gérer des sémaphore pour l'écriture sur le socket.
int recv_cli_packet(int sockfd, client_packet *packet)
{
    int n=0, nattempts=0;

    while (n<=0) {
        n = recv(sockfd, (void*)packet, sizeof(client_packet), 0);
        
        if (++nattempts > 5) {
            return -1;
        }
    }
    return 0;
}

//TODO: gérer des sémaphore pour l'écriture sur le socket.
int send_srv_packet(int sockfd, server_packet *packet)
{
    int n=0, nattempts=0;

    while (n<=0) {
        n = send(sockfd, (void*)packet, sizeof(server_packet), 0);

        if (++nattempts > 5) {
            return -1;
        }
    }
    return 0;
}

int get_options(int argc, char *argv[])
{
    int opt;
    
    static const struct option longopts[] = {
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

    receive_connections();
    return 0;
}
