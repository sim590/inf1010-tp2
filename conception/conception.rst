Client
------

- affiche les messages reçus:
    - les résultats des commandes
        -> affichage de la liste pour la première connexion
    - les messages des autres utilisateurs
    - les connexions et déconnexions, ex: fred a quitté

Commandes
=========
- connecter
- joindre un channel
- requête de 
    - d'une liste de clients dans le channel
    - de la liste des channels
- envoie de messages personnels

Serveur
-------

- réception des message des clients et redistribution de ceux-ci parmis les
  autres clients
- tient une table de connexion::

    type: liste (structures) chaînée (ordonnancer les noeuds par id-channel)
    
    Nom client | adresse ip    | port client | id channel |
    --------------------------------------------------------
    simon      | 192.168.1.103 | 8760        | 2          |
    fred       | 192.168.1.104 | 12367       | 2          |

- tient une table de channels::

    id channel | nom channel     | topic
    --------------------------------------------------
    2          | leshippiesLinux | Gloire à GNU/Linux
    2          | lesbonbons      | miam
