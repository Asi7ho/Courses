  1                   # TEST_RETURN_CODE=PASS
  2                   ## Fichier mult.s : un programme qui fait une division et une multiplication
  3                   .set noreorder        #pas de réordonnancement
  4                   .text
  5
  6                   ##----------------------------------------------------------------------------
  7                   # initialise les variables
  8 00000000 20190054   ADDI  $t9,$zero,EXIT
  9 00000004 200AFFD5   addi  $t2,$zero,-	43     	# $t2 <- -43
 10 00000008 200BFFFF   addi  $t3, $0,0xffff      	# $t3 <- 0xff
 11
 12                   #fait la division
 13 0000000C 014B001A   DIV  $t2,$t3			# divise les deux nombres
 14 00000010 00004012   mflo $t0 			# prend le quotient du résultat de la division
 15 00000014 00004810   mfhi $t1 			# prend le reste du résultat de la division
 16 00000018 11200009 BEQ $t1,$zero, 36           # si il n'y a pas de reste alors on peut tester dans l'autre sens
 17 0000001C 00005020   add $t2,$zero,$zero		# si pas réussi on set $t2 à 0
 18 00000020 0AA00000   J EXIT			# saut à la sortie sinon  		
 19
 20                   #fait la multiplication (remarquez le nom de l'étiquette)
 21
 22                   mult:
 23 00000024 00000000   NOP				# quelques non opérations pour respecter la consigne 
 24 00000028 00000000   nop				# de la doc concernant le MFHI
 25 0000002C 010B0018   mult $t0,$t3			# on essaye de retrouver le nombre de départ
 26 00000030 00004012   MFLO $t0 			# prend la partie basse de la multiplication
 27 00000034 00004810   MFHI $t1 			# prend la partie haute de la multiplication
 28 00000038 00006820   add $t5,$zero,$zero		# astuce pour éviter les optimisation de boucle
 29 0000003C 152D0015   BNE $t1,$t5,EXIT		# si le résultat est trop grand on sort
 30 00000040 110A0013   BEQ $t0,$t2,reussi		# si $t2 et $t0 sont égaux on a retrouvé le résultat
 31 00000044 00005020   add $t2,$zero,$zero		# si pas réussi on set $t2 à 0 et on sort
 32 00000048 0AA00000   J EXIT			
 33                   reussi:
 34 0000004C 200A0001   addi $t2,$zero,0x1		# si réussi on set $t2 à 1
 35 00000050 0AA00000   J EXIT			# et on sort
 36
 37
 38                   EXIT	:
 39 00000054 0AA00000     J EXIT
 40
 41                   .data
 42 00000000 0CAABBCC .byte 12,0xAA,0xBB,0xCC,0xdd
 42 00000004 000000DD
 43 00000005 000000FF .byte 0xFF
 44 00000006 AABBCCDD .word 0xAABBCCDD

.symtab
 22    .text:00000024 mult
 33    .text:0000004C reussi
 38    .text:00000054 EXIT

rel.text
00000000        R_MIPS_16        .text:00000054  EXIT
00000020        R_MIPS_26        .text:00000054  EXIT
0000003C        R_MIPS_16        .text:00000054  EXIT
00000040        R_MIPS_16        .text:0000004C  reussi
00000048        R_MIPS_26        .text:00000054  EXIT
00000050        R_MIPS_26        .text:00000054  EXIT
00000054        R_MIPS_26        .text:00000054  EXIT


rel.data
