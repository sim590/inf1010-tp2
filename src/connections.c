//-------------------------------------------------------
// File: connections.c    Author(s): Simon Désaulniers
//                                   Frédéric Hamelin
// Date: 2013-11-06
//
// © Copyright 2013 Simon Désaulniers, 
//                  Frédéric Hamelin. All Rights Reserved.
//-------------------------------------------------------
// Code de la gestion des connexion.
//-------------------------------------------------------

#include <connections.h>

connection *first_con = NULL;
connection *last_con = NULL;

connection* add_connection(char id[], int sockfd, struct sockaddr_in addr , int tid)
{
    // ne peut ajouter deux fois la même connexion
    if (!find_connection(id, NULL))
        return NULL;

    connection *new_con, *prev_con;
    // impossible d'allouer de la mémoire.
    if (!(new_con = malloc(sizeof(connection))))
        return NULL;

    *new_con = (connection) {
        .channel_id = DEFAULT_CHANNEL_ID, 
        .sockfd = sockfd,
        .addr = addr,
        .tid = tid,
        .next = NULL, 
        .prev = NULL 
    };
    strncpy(new_con->id, id, 32);

    // liste vide.
    if (!first_con && !last_con) {
        first_con = last_con = new_con;
        return new_con;
    }

    prev_con->next = new_con;
    new_con->prev = prev_con;

    return new_con;
}

int remove_connection(char id[])
{
    // si vide
    if (!first_con || !last_con) {
        return -1;
    }

    // si un seul élément
    if (first_con == last_con && !strcmp(first_con->id, id)) {
        free(first_con);
        first_con = last_con = NULL;
        return 0;
    }

    connection *con;
    if (find_connection(id, &con)) {
        // au début
        if (con == first_con) {
            con->next->prev = NULL;
        }
        // à la fin
        else if (con == last_con) {
            con->prev->next = NULL;
        }
        else {
            con->prev->next = con->next;
            con->next->prev = con->prev;
        }
        free(con);
    }
    
    return -1;
}

int find_connection(char id[], connection **con)
{
    int equals = 0;
    connection *cur = first_con;
    while (cur) {
        if (!strcmp(id, cur->channel_id)) {
            if (con)
                *con = cur;
            return 0;
        }
        cur = cur->next;
    }

    return -1;
}
