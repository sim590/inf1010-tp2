//-------------------------------------------------------
// File: channels.c    Author(s): Simon Désaulniers
//                                Frédéric Hamelin
// Date: 2013-11-05
//
// © Copyright 2013 Simon Désaulniers, 
//                  Frédéric Hamelin. All Rights Reserved.
//-------------------------------------------------------
// List of channels management.
//-------------------------------------------------------

#include <channels.h>

channel *first_chan = NULL;
channel *last_chan = NULL;

int add_channel(char id[], char topic[])
{
    //Il n'existe qu'un canal pour chaque id de canal
    if (!find_channel(id, NULL))
        return -1;

    channel *new_chan;
    // impossible d'allouer de la mémoire.
    if (!(new_chan = malloc(sizeof(channel))))
        return -2;

    //construction du nouveau channel
    *new_chan = (channel) {.next = NULL, .prev = NULL};
    strncpy(new_chan->id, id, 32);
    strncpy(new_chan->topic, topic, 256);

    //si la liste est vide
    if (!first_chan && !last_chan) {
        first_chan = last_chan = new_chan;
    }
    //sinon, on ajoute à la fin
    else {
        last_chan->next = new_chan;
        new_chan->prev = last_chan;
        last_chan = new_chan;
    }
    return 0;
}

int remove_channel(char id[])
{
    // si vide
    if (!first_chan || !last_chan) {
        return -1;
    }

    // si un seul élément
    if (first_chan == last_chan && !strcmp(last_chan->id, id)) {
        free(first_chan);
        first_chan = last_chan = NULL;
        return 0;
    }

    channel *chan;
    // si le canal existe, on le supprime
    if (!find_channel(id, &chan)) {
        // à la fin
        if (chan == last_chan) {
            last_chan->prev->next = NULL;
        }
        // au début
        else if (chan == first_chan) {
            first_chan = chan->next;
        }
        else {
            chan->prev->next = chan->next;
            chan->next->prev = chan->prev;
        }

        free(chan);
        return 0;
    }
    return -1;
}

int find_channel(char id[], channel **chan)
{
    channel *cur = first_chan;
    while (cur) {
        if (!strcmp(cur->id, id)) {
            if (chan)
                *chan = cur;
            return 0;
        }
        cur = cur->next;
    }
    if (chan)
        *chan = NULL;
    return -1;
}
