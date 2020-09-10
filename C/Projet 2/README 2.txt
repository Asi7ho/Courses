simpleUnitTest.sh : script de test "unitaire" "boite noire" (ou "externe") minimal.

########################################
# Update Sept 2012
# Update August 2016
# Modified November 2017
########################################
# No restriction on usage nor dissemination
########################################
# Problèmes, commentaires : francois.portet@phelma.grenoble-inp.fr ou nicolas.castagne@phelma.grenoble-inp.fr
########################################

########################################
# Lancement rapide des tests 
########################################
1/ se placer dans le répertoire de l'exécutable a tester
2/ entrer, par exemple
	../testing/simpleUnitTest.sh -e ./as-mips tests/*.s

Où : 
	../testing/simpleUnitTest.sh est le chemin vers script de test
	./as-mips est le chemin vers l'executable a tester  
	test/*.s  designe l'ensemble des fichiers .s à tester

Variante, mode batch, avec l’option -b : tous les tests sont lancés immédiatement, jusqu’au rapport final :
	../testing/simpleUnitTest.sh -e ./as-mips -b tests/*.s



Voir la suite pour des détails.


########################################
# Introduction
########################################
Le script lance un <executable> sur une liste de fichiers de <test>, l'ensemble étant passé en argument du script.


===< Chaque fichier de <test>.s doit débuter par un COMMENTAIRE ( # BLA BLA  ) qui :
1- précise si ce test doit générer une erreur (le programme testé est censé retourné un code erreur), au moyen de :
	; TEST_RETURN_CODE=PASS
ou	; TEST_RETURN_CODE=FAIL
ou	; TEST_RETURN_CODE=DO_NOT_CHECK

Dans le premier cas PASS, on attend que le programme testé retourne 0
Dans le premier cas FAIL, on attend que le programme testé retourne un nombre différent de 0.
C’est donc que le programme testé est censé générer une erreur pour le test considéré.
Dans le dernier cas DO_NOT_CHECK, le code de retour du programme testé ne sera pas considéré.


2- donne un éventuel commentaire sur le test, précédé de TEST_COMMENT=



==> chaque fichier <test>.s doit être accompagné d'un fichier <test>.res
	Ce fichier doit contenir la sortie attendue - ce qu'est censé générer l'<executable> 
	sur son fichier de sortie quand il est exécuté sur le fichier <test>.
	C'est ce fichier <test>.res qui est utilisé pour comparer le fichier de sortie attendu avec le fichier généré par l'<exécutable>.



Pour chacun des fichiers de <test> passé en argument, le script : 
	- lance l'<executable> en lui passant <test> en argument
	- détecte si l'<executable> a "planté" (segmentation fault, etc.)
	- sinon, détecte si le code de sortie renvoyé par l'<exécutable> correspond au code erreur attendu, donné dans le fichier .info
	- et détecte si la sortie de l'<executable> correspond à la sortie attendue, spécifiée dans le fichier .res

Enfin, le script génère un rapport de test avec les résultats de tous les tests.

