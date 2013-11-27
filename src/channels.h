//-------------------------------------------------------
// File: channels.h    Author(s): Simon Désaulniers
//                                Frédéric Hamelin
// Date: 2013-11-06
//
// © Copyright 2013 Simon Désaulniers, 
//                  Frédéric Hamelin. All Rights Reserved.
//-------------------------------------------------------
// Fichier d'entête du code de gestion des cannaux du 
// chat.
//-------------------------------------------------------
#ifndef CHANNELS_S7PS9ZKO

#define CHANNELS_S7PS9ZKO

#include <common.h>

//----------------------------
//----------------------------
// Liste de channels ouverts
//----------------------------
//----------------------------
typedef struct _channel {
    char id[32];
    char topic[256];
    struct _channel *next;
    struct _channel *prev;
} channel;

//----------------------
// add_channel()
// valeurs de retour:
//  -2: impossible d'allouer de
//      la mémoire.
//  -1: il existe déjà un canal 
//      avec ce nom.
//  0: succès
// Ajoute un canal à la liste de
// cannaux en spécifiant les informations
// suivantes:
//  - id;
//  - topic.
int add_channel(char[], char[]);

//-------------------
// remove_channel()
// valeurs de retour:
//  -1: échec
//  0: succès
// Retire un canal de la liste de 
// cannaux.
int remove_channel(char[]);

//-----------------
// find_channel()
// valeurs de retour:
//  -1: échec
//  0: succès
// Trouve le channel selon son id. Le pointeur
// se verra attribuer l'adresse du noeud trouvé. 
// S'il y a échec, le pointeur se verra attribuer
// NULL.
int find_channel(char[], channel**);


#endif /* end of include guard: CHANNELS_S7PS9ZKO */
