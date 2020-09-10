#TEST_RETURN_CODE=PASS
.set noreorder
.text
    ADD $2, $3, $4
    ADDI $2, $3, 34
boucle:
    ADD $2, $3, $4
.data
    .word boucle
