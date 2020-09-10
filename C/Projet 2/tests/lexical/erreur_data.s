#TEST_RETURN_CODE=FAIL

#erreur addition

.set noreorder
.text
    LW $t0, 1a
    LW $t1, 2b
    LW $t2, res
    ADD res, a, b
.data
1a:
    .word 5
2b:
    .word 2
res:
    .word 0
