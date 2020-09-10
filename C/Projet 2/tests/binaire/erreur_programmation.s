#TEST_RETURN_CODE=FAIL
erreur de programmation

.set noreorder
.text
    LW $t0, a
    LW $t1, b
    LW $t2, res
    ADD res, a, b
.data
a:
    .word 5
b:
    .word 07
res:
    .word 0
