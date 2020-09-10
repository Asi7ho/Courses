# TEST_RETURN_CODE=PASS
## Fichier mult.s : un programme qui fait une division et une multiplication
.set noreorder        #pas de réordonnancement
.text

##----------------------------------------------------------------------------
# initialise les variables
  ADDI  $t9,$zero,EXIT
  addi  $t2,$zero,-	43     	# $t2 <- -43
  addi  $t3, $0,0xffff      	# $t3 <- 0xff

#fait la division
  DIV  $t2,$t3			# divise les deux nombres
  mflo $t0 			# prend le quotient du résultat de la division
  mfhi $t1 			# prend le reste du résultat de la division
BEQ $t1,$zero, 36           # si il n'y a pas de reste alors on peut tester dans l'autre sens
  add $t2,$zero,$zero		# si pas réussi on set $t2 à 0
  J EXIT			# saut à la sortie sinon  		

#fait la multiplication (remarquez le nom de l'étiquette)

mult:
  NOP				# quelques non opérations pour respecter la consigne 
  nop				# de la doc concernant le MFHI
  mult $t0,$t3			# on essaye de retrouver le nombre de départ
  MFLO $t0 			# prend la partie basse de la multiplication
  MFHI $t1 			# prend la partie haute de la multiplication
  add $t5,$zero,$zero		# astuce pour éviter les optimisation de boucle
  BNE $t1,$t5,EXIT		# si le résultat est trop grand on sort
  BEQ $t0,$t2,reussi		# si $t2 et $t0 sont égaux on a retrouvé le résultat
  add $t2,$zero,$zero		# si pas réussi on set $t2 à 0 et on sort
  J EXIT			
reussi:
  addi $t2,$zero,0x1		# si réussi on set $t2 à 1
  J EXIT			# et on sort


EXIT	:
    J EXIT

.data
.byte 12,0xAA,0xBB,0xCC,0xdd
.byte 0xFF
.word 0xAABBCCDD
