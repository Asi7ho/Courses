Le code se lance à travers le terminal : 

- Lancer le terminal et se placer dans le dossier où se trouve le code.
	cd /users/phelma/phelma2016/tdinfo/LOGIN/tdinfo/seance15

- La compilation du code se fait à l’aide d’un Makefile
	make

- Pour lancer le code, tapez dans le terminal:
	./main

	


La configuration par défaut est : 

	fichier par défaut :  NOM_DU_FICHIER = « graphe11.txt »
	
	paramètres par défaut : NOMBRE_DE_FOURMIS = 2
				COEFFICIENT_EVAPORATION_PHEROMONE = 0.5
				IMPORTANCE_PHEROMONE = 1
				IMPORTANCE_VISIBILITEE = 2
				VALEUR_INITIALE_PHEROMONE = 1E-5
				QUANTITE_PHEROMONE_A_DEPOSEE = 1
				MAX_CYCLE  = 1




Pour changer les paramètres :   - se rendre dans le répertoire du code
				- ouvrir le fichier « fourmis.h »
				- changer les paramètres

Pour changer de fichier de graphe :     - Mettre le fichier voulu dans le fichier où se 					trouve les codes
					-  Ouvrir le fichier « main.c »
					-  changer le nom du fichier

Après chaque modifications dans les fichiers .h, pensez à faire dans le terminal:
	make clean
