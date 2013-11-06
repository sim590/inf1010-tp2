//-------------------------------------------------------
// File: channels.c    Author(s): Simon Désaulniers
//                                Frédéric Hamelin
// Date: 2013-11-05
//-------------------------------------------------------
// List of channels management.
//-------------------------------------------------------

#include <server.h>

channel *first = NULL;
channel *last = NULL;

int add_channel(char id[], char topic[])
{

    //Il n'existe qu'un canal pour chaque id de canal
    if (!find_channel(id, &DUMMY, &DUMMY)) {
        return -1;
    }

    channel *new_chan;
    if (!(new_chan = malloc(sizeof(channel)))) {
        // impossible d'allouer de la mémoire.
        return -1;
    }

    //construction du nouveau channel
    new_chan->next = NULL;
    strncpy(&new_chan->id, &id, 32);
    strncpy(&new_chan->topic, &topic, 256);

    //si la liste est vide
    if (first == NULL && last == NULL) {
        first = last = new_chan;
    }
    //sinon, on ajoute à la fin
    else {
        last->next = new_chan;
        last = new_chan;
    }
    return 0;
}

int remove_channel(char id[])
{
    channel *chan, *prev;

    // si le canal existe, on le supprime
    if (!find_channel(id, &chan, &prev)) {
        if (chan != first) {
            prev->next = chan->next;
        }
        else {
            first = chan->next;
        }
        free(chan);
        return 0;
    }
    return -1;
}

int find_channel(char id[], channel **chan, channel **previous)
{
    if (!chan || !previous) {
        fprintf(stderr, "NULL pointer passed to find_channel()\n");
        return -1;
    }
    channel *prev, *cur;
    prev = cur = first;

    while (cur) {
        if (!strcmp(&cur->id, &id)) {
            *chan = cur;
            *previous = prev;
            return 0;
        }
        prev = cur;
        cur = cur->next;
    }
    *chan = NULL;
    return -1;
}
