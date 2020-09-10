#TEST_RETURN_CODE=FAIL

.set noreorder
.text
    LW $t0, a
    LW $t1, b
    LW $t2, res
    ADD res, a, b
.data
a:
    .word 08 #erreur
b:
    .word 2
res:
    .word 0
