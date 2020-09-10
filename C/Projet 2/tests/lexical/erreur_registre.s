#TEST_RETURN_CODE=FAIL

.set noreorder
.text
    LW $t0, a
    LW $t1, b
    LW $48, res
    ADD res, a, b
.data
a:
    .word 5
b:
    .word 07
res:
    .word 0
