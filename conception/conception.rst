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
     message.
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


Serveur
-------

- réception des message des clients et redistribution de ceux-ci parmis les
  autres clients

- tient une table de connexion::

    type: liste doublement chaînée (ordonnancé selon le id channel)

    id utilisateur | id channel | Nom utilisateur | adresse ip    | port client
    -----------------------------------------------------------------------------
    0              | 1          | simon           | 192.168.1.103 | 8760
    1              | 1          | fred            | 192.168.1.104 | 12367
    2              | 0          | mathieu         | 24.122.22.105 | 4329
    3              | 1          | yannick         | 192.168.1.101 | 5239

- tient une table de channels::

    id channel | nom channel     | topic              
    --------------------------------------------------
    0          | lesbonbons      | miam               
    1          | leshippiesLinux | Gloire à GNU/Linux 

    Mise en situation: Un client 3 envoie un message dans son channel.

        - Le serveur identifie le client et le channel dans lequel il se trouve
          (à l'aide de la table de connexions).
        - Le serveur envoie le message à tous ceux qui se trouvent autour du
          client dans la liste chaînée jusqu'à ce que le id channel ne soit plus
          le même:

    Mise en situation: Un client 0 demande la liste des utilisateurs dans son channel.

        - Le serveur identifie le client et le channel dans lequel il se trouve
          (à l'aide de la table de connexions).
        - Le serveur boucle à gauche et à droite pour amasser la liste des
          clients dans le channel du demandeur et la lui envoie.

         ___________________         __________________         __________________         __________________         __________________      
        |                    |     |                    |     |                    |     |                    |     |                    |
        | id utilisateur: 1  |     | id utilisateur: 0  |     | id utilisateur: 3  |     | id utilisateur: 2  |     | id utilisateur: 4  |
        | id channel: 0      |     | id channel: 1      |     | id channel: 1      |     | id channel: 1      |     | id channel: 4      |
    ... | noeud* suivant     | --> | noeud* suivant     | --> | noeud* suivant     | --> | noeud* suivant     | --> | noeud* suivant     | ...
        | noeud* precedent   | <-- | noeud* precedent   | <-- | noeud* precedent   | <-- | noeud* precedent   | <-- | noeud* precedent   |
        | __________________ |     | __________________ |     | __________________ |     | __________________ |     | __________________ |
                                                                                                                                              
                                    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< CLIENT-3 ^ >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
                                    Recherche par la gauche et par la droite jusqu'à ce que le id channel ne 
                                    soit plus ``1`` puisqu'on sait que la liste de clients dans un même 
                                    channel sont adjacents.

