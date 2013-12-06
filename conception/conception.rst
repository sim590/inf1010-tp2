Client
------

- affiche les messages reçus:
    - les résultats des commandes (affichage de la liste pour la première
      connexion)
    - les messages des autres utilisateurs
    - les connexions et déconnexions, ex: fred a quitté

Commandes
=========

Le client a à sa dispositions ces commandes qu'il peut envoyer au serveur, après
quoi celui-ci fera le traitement et renvera le résultat.

- msg::
    description: Envoie un message à l'utilisateur spécifié.
    synopsis: /msg <nom_utilisateur> <message>
     - nom_utilisateur: Le nom d'utilisateur de la personne à qui envoyer le
                        message. Si le nom d'utilisateur est "-", envoyer à tous
                        les clients connectés.
     - message: Le message à envoyer.

- names::
    synopsis: /names
    description: Requête de la liste des clients connectés dans le canal.

- list::
    description: Requête de la liste de canaux disponibles.
    synopsis: /list

- connect::
    description: Connecte le client au serveur spécifié.
    synopsis: /connect <nome_hôte>
     - nome_hôte: Le nom d'ĥôte du serveur auquel se connecter.

- join::
    description: Change le canal dans lequel le client clavarde.
    synopsis: /join <nom_canal>
     - nom_canal: Le nom du canal auquel se joindre.

- disconnect::
    description: Déconnecte le client du serveur.
    synopsis: /disconnect

- quit::
    description: Ferme le programme.
    synopsis: /quit

**Lorsqu'il y a un retour du serveur, cela consiste toujours en une liste de
caractères à afficher à l'écran du client.**


Serveur
-------

- réception des message des clients et redistribution de ceux-ci parmis les
  autres clients

- tient une table de connexion::

    type: liste doublement chaînée (ordonnancé selon le id channel)

    id utilisateur | id channel      | adresse ip    | port client
    ----------------------------------------------------------------
    simon          | leshippiesLinux | 192.168.1.103 | 8760
    fred           | leshippiesLinux | 192.168.1.104 | 12367
    mathieu        | lesbonsbons     | 24.122.22.105 | 4329
    yannick        | leshippiesLinux | 192.168.1.101 | 5239

- tient une table de channels::

    id channel      | topic
    --------------------------------------
    lesbonbons      | miam
    leshippiesLinux | Gloire à GNU/Linux

Mises en situation
==================

- Le client 3 envoie un message dans son channel::
  
    input: texte

    - Le serveur identifie le client et le channel dans lequel il se trouve
      (à l'aide de la table de connexions).
    - Le serveur envoie le message à tous ceux qui se trouvent autour du
      client dans la liste chaînée jusqu'à ce que le id channel ne soit plus
      le même:

- Le client 0 demande la liste des utilisateurs dans son channel::

    input: /names

    - Le serveur identifie le client et le channel dans lequel il se trouve
      (à l'aide de la table de connexions).
    - Le serveur boucle à gauche et à droite pour amasser la liste des
      clients dans le channel du demandeur et la lui envoie.

Schéma représentant les deux situations ci-haut::
    ____________________         __________________         __________________         __________________         __________________      
    |                    |     |                    |     |                    |     |                    |     |                    |
    | id utilisateur: 1  |     | id utilisateur: 0  |     | id utilisateur: 3  |     | id utilisateur: 2  |     | id utilisateur: 4  |
    | id channel: 0      |     | id channel: 1      |     | id channel: 1      |     | id channel: 1      |     | id channel: 4      |
    | noeud* suivant     | --> | noeud* suivant     | --> | noeud* suivant     | --> | noeud* suivant     | --> | noeud* suivant     | ...
    | noeud* precedent   | <-- | noeud* precedent   | <-- | noeud* precedent   | <-- | noeud* precedent   | <-- | noeud* precedent   |
    | __________________ |     | __________________ |     | __________________ |     | __________________ |     | __________________ |
                                                                                                                                              
                                <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< CLIENT-3 ^ >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
                                Recherche par la gauche et par la droite jusqu'à ce que le id channel ne 
                                soit plus ``1`` puisqu'on sait que la liste de clients dans un même 
                                channel sont adjacents.

- Le client 1 demande la liste des cannaux ouverts sur le serveur::

    input: /list

    - Le serveur parcours la liste de cannaux en entier, il accumule la liste
      et l'envoie au client.

- Un client se connecte au serveur::

  informations préalables
  -------------------------
    - hostname: 192.168.1.2

  input: /connect 192.168.1.2

  - Le serveur reçoit la demande et lance l'ajout des informations du client
    dans la table de connexions;
  - Place le client dans le canal par défaut (id: 0) et lui envoie la liste de
    gens connectés sur ce canal.

- Un client joint un autre canal::

  informations préalables
  -------------------------
    - nom_channel: toto

  input: /join toto

  - Le serveur cherche le client dans la table de connexion et change son
    identifiant "id channel" pour le nouveau.

- Un client se déconnecte::

  input: /disconnect

  - Le serveur retire le noeud de connexion de l'utilisateur se déconnectant en
    le recherchant par user_id.
  - Envoie un message dans le canal dans lequel l'utilisateur était afin
    d'informer les autres que l'utilisateur est parti.
