# CY_DVENTURE
Mini jeu en c qui déroule une histoire.


Se base sur la lecture de plusieurs fichiers .txt.

Chaque fichier texte est composé de 5 blocs :
bloc 1 : L'histoire qui doit être précédée de "# Description#
bloc 2 : L'evenement qui est soit un bonus, un tirage au sort ou un monstre à combattre. 1 seule choix d'évenement est possible et doit être précédé d'un des tag suivant : "# Evenement" ou "# Bonus" ou "# Charlatant" ou "# Monstre"
bloc 3 : Choix de la suite de l'histoire ou combat contre le boss final. Précédé d'un des tags suivant : "# Choix" ou "# Combat final"

Une seule sauvegarde par joueur est possible. Si le joueur meurt ou gagne face au boss alors sa sauvegarde esr supprimée.

L'histoire est divisée en plusieur étape. On commance à l'étape 1. Il y aura divers choix : 1, 2... Si le choix se porte sur le 2 alors on passera à l'étape 12 puis si on choisi 1 alors on sera à l'étape 121, etc...

BUG : Non dérangant mais a chaque fois qu'une étape est lu (un fichier contenant l'histoire), un fichier portant le numéro de l'étape sans extension et contenant les infos du joueur est crée.

Lancer le programme : Dans l'invite de commance, au niveau du répertoire courant. Lancer la commande: make puis make run.
