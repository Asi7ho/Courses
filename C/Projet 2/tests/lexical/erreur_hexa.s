#TEST_RETURN_CODE=FAIL

.set noreorder
.text
    LW $t0, a
    LW $t1, b
    LW $t2, res
    ADD $t2, $t0, $t1
.data
a:
    .word 0xg #erreur
b:
    .word 2
res:
    .word 0
