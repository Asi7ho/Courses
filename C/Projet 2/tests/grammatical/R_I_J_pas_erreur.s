#TEST_RETURN_CODE=PASS

.text
    boucle:
        ADD $2, $3, $4
        ADDI $2, $3, 45
        J boucle
