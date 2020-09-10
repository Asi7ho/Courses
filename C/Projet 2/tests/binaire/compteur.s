#TEST_RETURN_CODE=PASS
# compteur

.set noreorder
.text
    LW $t0, compteur
    LW $t1, fin_compteur
boucle:
    BEQ $t0, $t1, end
    NOP
    ADDI $t0, $t0, 1
    J boucle
    NOP
end:
    J end
.data
    compteur:
        .word 1
    fin_compteur:
        .word 100
