#TEST_RETURN_CODE=DO_NOT_CHECK
# allons au ru

#Espace debut de ligne impossible
.set noreorder
.text

#Pas d'espace apres la virgule
LW $t0 ,lunchtime

#Espace après parenthèse et tabulation après parenthèses
LW $6, -0x200( $7    )

#registre avec un espace impossible
ADDI $ t1,$zero,8

#boucle suivi de 2 tabulations
boucle      :
    BEQ $t0 , $t1 , byebye
    NOP
    addi $t1 , $t1 , 1
    J boucle
    NOP

#instruction avec espace entre instruction et : impossible
byebye :
    JAL viteviteauru

.data
lunchtime:
    .word 12
    .word menu

#space entre directive et . impossible
.   bss
menu:
    .space 24
