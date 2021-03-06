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

extern sem_t *cons_sem;
extern connection *first_con;
extern channel *first_chan;

int init_socket()
{
    // Création du scoket de réception
    reception_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (reception_socket_fd <= 0) {
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
    socklen_t clilen = sizeof(struct sockaddr), sockfd;
    pthread_t tid;

    if (listen(reception_socket_fd, 5)) {
        perror("Erreur lors de la mise en écoute sur le socket.");
        exit(EXIT_FAILURE);
    }

    cons_sem = malloc(sizeof(sem_t));
    if (!cons_sem || sem_init(cons_sem, 0, 1)) {
        fprintf(stderr, "Impossible d'initialiser la liste de connexion..\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        sockfd = accept(reception_socket_fd, (struct sockaddr*) &cli_addr, &clilen);

        hcc_args *args = malloc(sizeof(hcc_args));
        *args = (hcc_args) {sockfd, cli_addr};

        
        if (pthread_create(&tid, NULL, handle_client_communication, args))
            fprintf(stderr, "Impossible de créer un thread de connexion pour le client %u.%u.%u.%u...\n", 
                    (cli_addr.sin_addr.s_addr<<24)>>24,
                    (cli_addr.sin_addr.s_addr<<16)>>24,
                    (cli_addr.sin_addr.s_addr<<8)>>24,
                    cli_addr.sin_addr.s_addr>>24
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
    if (recv(sockfd, (void*)&cli_packet, sizeof(client_packet), 0) <= 0) {
        free(args);
        close(sockfd);
        return NULL;
    }

    // ajout de la connexion avec le client à la liste
    connection *cur_con, *my_con = add_connection(
                cli_packet.con_info.id, 
                sockfd,
                args->cli_addr, 
                my_tid
            );
    channel *cur_chan;

    // ne peut ajouter la connexion avec le client à la liste.
    if (!my_con) {
        srv_packet.type = -1;
        send(sockfd, (void*)&srv_packet, sizeof(server_packet), 0);
    }
    else {
        // confirmation du succès
        srv_packet = (server_packet) {.type = 0};
        if (send_srv_packet(my_con, &srv_packet)) {
            close_connection(my_con, args);
            close(sockfd);
            return NULL;
        }

        // s'occuper des requêtes du clients
        while (1) {
            memset(&srv_packet, 0, sizeof(server_packet));

            if (recv_cli_packet(my_con, &cli_packet)) {
                close_connection(my_con, args);
                return NULL;
            }

            switch(cli_packet.type) {
                // ---------------------------------------
                // envoie d'un message au canal du client
                // --------------------------------------- ***testé
                case CLI_TXT:
                    cur_con = first_con;
                    srv_packet = (server_packet){
                        .msg = {
                        .type = SRV_TXT,
                        .priv = 0
                        }
                    };

                    strcpy(srv_packet.msg.from, my_con->id);
                    strcpy(srv_packet.msg.message, cli_packet.msg.message);

                    while (cur_con) {
                        // send sur le socket du client cur.
                        if (cur_con != my_con && !strcmp(cur_con->channel_id, my_con->channel_id))
                            send_srv_packet(cur_con, &srv_packet);
                        cur_con = cur_con->next;
                    }
                    break;
                // ---------------------------------
                // commandes: /cmd <arg1> <arg2> ...
                // ---------------------------------
                case CLI_CMD:
                    cur_con = first_con;

                    // ---------------------------------
                    // /msg <nom_utilisateur> <message>
                    // --------------------------------- ***testé
                    if (!strcmp(cli_packet.cmd.args[0], "msg")) {
                    srv_packet = (server_packet){
                        .msg = {
                        .type = SRV_TXT,
                        .priv = 1
                        }
                    };
                        strcpy(srv_packet.msg.from, my_con->id);
                        strcpy(srv_packet.msg.message, cli_packet.cmd.main_arg);

                        // message à tous les clients connectés au serveur
                        if (!strcmp(cli_packet.cmd.args[1], "-")) {
                            while (cur_con) {
                                if (cur_con != my_con)
                                    send_srv_packet(cur_con, &srv_packet);
                                cur_con = cur_con->next;
                            }
                        }
                        // message à un client en particulier
                        else if (!find_connection(cli_packet.cmd.args[1], &cur_con))
                            send_srv_packet(cur_con, &srv_packet);
                    }

                    // -------
                    // /names
                    // -------  ***testé
                    else if (!strcmp(cli_packet.cmd.args[0], "names")) {
                        srv_packet.type = SRV_TXT;
                        memset(srv_packet.msg.message, 0, MESSAGE_SIZE);

                        while (cur_con) {
                            if (!strcmp(cur_con->channel_id, my_con->channel_id)) {
                                strcat(srv_packet.msg.message, cur_con->id);
                                if (cur_con->next)
                                    strcat(srv_packet.msg.message, ":");
                            }
                            cur_con = cur_con->next;
                        }
                        send_srv_packet(my_con, &srv_packet);
                    }

                    // ------
                    // /list
                    // ------ ***testé
                    else if (!strcmp(cli_packet.cmd.args[0], "list")) {
                        cur_chan = first_chan;

                        srv_packet.type = SRV_TXT;
                        memset(srv_packet.msg.message, 0, MESSAGE_SIZE);

                        while (cur_chan) {
                            // contenu de la forme:
                            //
                            //      channel1\Nous sommes dans le canal 1
                            //      channel2\Le canal deux (2), c'est le meilleur!
                            //
                            strcat(srv_packet.msg.message, cur_chan->id);
                            strcat(srv_packet.msg.message, "\\");
                            strcat(srv_packet.msg.message, cur_chan->topic);
                            strcat(srv_packet.msg.message, "\n");
                            
                            cur_chan = cur_chan->next;
                        }
                        send_srv_packet(my_con, &srv_packet);
                    }

                    // -------------------
                    // /join <nom_channel>
                    // ------------------- ***testé
                    else if (!strcmp(cli_packet.cmd.args[0], "join")) {
                        char topic[256];
                        memset(topic, 0, 256);
                        strcat(topic, my_con->id);
                        strcat(topic, ": vient de créer le canal.");

                        wait_for_connection(my_con);
                        strcpy(my_con->channel_id, cli_packet.cmd.args[1]);
                        sem_post(my_con->sem);

                        /*TODO: gérer sémaphores channels*/
                        add_channel(cli_packet.cmd.args[1],topic);
                    }

                    // -----------
                    // /disconnect 
                    // -----------
                    else if (!strcmp(cli_packet.cmd.args[0], "disconnect")) {
                        close_connection(my_con, args);
                        return NULL;
                    }
                    break;
                // mauvais paquet
                default:
                    break;
            }
        }
    }

    if (!my_con)
        close_connection(my_con, args);
    return NULL;
}

void close_connection(connection *con, hcc_args *args)
{
    free(args);
    close(con->sockfd);
    remove_connection(con->id);
}

int recv_cli_packet(connection *con, client_packet *packet)
{
    int n=0, nattempts=0;

    while (n<=0) {
        n = recv(con->sockfd, (void*)packet, sizeof(client_packet), 0);
        
        if (++nattempts > 5) {
            return -1;
        }
    }
    return 0;
}

int send_srv_packet(connection *con, server_packet *packet)
{
    int n=0, nattempts=0;

    while (n<=0) {
        n = send(con->sockfd, (void*)packet, sizeof(server_packet), 0);

        if (++nattempts > 5) {
            return -1;
        }
    }
    return 0;
}

void handle_sig()
{
    close(reception_socket_fd);
    exit(EXIT_SUCCESS);
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
    return 0;
}

int main(int argc, char *argv[])
{
    signal(SIGINT, handle_sig);
    signal(SIGKILL, handle_sig);
    
    if(get_options(argc, argv))
        exit(EXIT_FAILURE);

    if (init_socket()) {
        fprintf(stderr, "Erreur lors de l'initialisation du serveur...\n");
        exit(EXIT_FAILURE);
    }

    // ajout du canal par défaut
    add_channel(DEFAULT_CHANNEL_ID, DEFAULT_CHANNEL_TOPIC);
    receive_connections();
    return 0;
}
