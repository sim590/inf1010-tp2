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

sem_t *cons_sem;
connection *first_con = NULL;
connection *last_con = NULL;

int wait_for_connection(connection *con)
{
    if (!con || con->flagged_deleted)
        return -1;
    sem_wait(con->sem);
    if (!con ) {
        return -1;
    }
    else if (con->flagged_deleted) {
        sem_post(con->sem);
        return -1;
    }
    return 0;
}

connection* add_connection(char id[], int sockfd, struct sockaddr_in addr , int tid)
{
    // ne peut ajouter deux fois la même connexion
    if (find_connection(id, NULL))
        return NULL;

    connection *new_con, *last = last_con;
    // impossible d'allouer de la mémoire.
    if (!(new_con = malloc(sizeof(connection))))
        return NULL;

    *new_con = (connection) {
        .channel_id = DEFAULT_CHANNEL_ID, 
        .sockfd = sockfd,
        .addr = addr,
        .tid = tid,
        .flagged_deleted = 0,
        .sem = malloc(sizeof(sem_t)),
        .next = NULL, 
        .prev = NULL 
    };
    if(sem_init(new_con->sem, 0, 1)) {
        free(new_con);
        return NULL;
    }
    strncpy(new_con->id, id, 32);

    // liste vide.
    sem_wait(cons_sem);
    if (!first_con && !last_con) {
        first_con = last_con = new_con;
        sem_post(cons_sem);
        return new_con;
    }
    sem_post(cons_sem);

     /*
      * On bloque le dernier noeud pour ajouter à la fin de la liste. Le
      * dernier noeud pourrait avoir été changé en attendant (sem_wait), alors
      * on doit vérifier si le dernier noeud est vraiment encore celui pour
      * lequel on attend.
      */
    while (1) {
        sem_wait(last->sem);

        if (last == last_con) {
            last->next = new_con;
            new_con->prev = last;

            sem_post(last->sem);
            return new_con;
        }
        // On a tout supprimé
        else if (!last_con) {
            sem_wait(cons_sem);
            // si toujours vide
            if (!last_con) {
                first_con = last_con = new_con;
                return new_con;
            }
            // si ajout depuis le wait()
            else
                last = last_con;
            sem_post(cons_sem);
        }
        // le last_con n'est plus le même depuis le dernier wait()
        else {
            if (last)
                sem_post(last->sem);
            last = last_con;
        }
    }
}

int remove_connection(char id[])
{
    // si vide
    if (!first_con || !last_con) {
        return -1;
    }

    // si un seul élément
    if (first_con == last_con && !strcmp(first_con->id, id)) {
        // on flag comme supprimé
        sem_wait(cons_sem);
        first_con->flagged_deleted = 1;

        // on attend que tous se rendent compte du flag.
        sem_post(cons_sem);
        sem_wait(cons_sem);

        free(first_con);
        first_con = last_con = NULL;
        return 0;
    }

    /*
     * TODO: faire attention aux boucles infini de sem_wait().. comment régler?
     * Un thread pourrait wait sur l'un l'autre et ça ne finirerait jamais...
     *
     * p. ex: on bloque la connexion ``con``, mais un autre thread bloque une
     * connexion autour (``con->prev`` ou ``con->next``) avant que le thread
     * qui a bloqué la connexion ``con`` ne le fasse.
     */
    connection *con;
    if (!find_connection(id, &con)) {
        if (wait_for_connection(con))
            return 0;
        // au début
        if (con == first_con) {
            wait_for_connection(con->next);

            con->next->prev = NULL;
            sem_post(con->next->sem);
        }
        // à la fin
        else if (con == last_con) {
            wait_for_connection(con->prev);

            con->prev->next = NULL;
            sem_post(con->next->sem);
        }
        else {
            wait_for_connection(con->prev);
            wait_for_connection(con->next);
            con->prev->next = con->next;
            con->next->prev = con->prev;
        }

        con->flagged_deleted = 1;
        // on attend que tous se rendent compte que c'est à être supprimé
        sem_post(con->sem);
        sem_wait(con->sem);
        free(con->sem);
        free(con);
        return 0;
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
