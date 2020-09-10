#TEST_RETURN_CODE=PASS
# addition octale

.set noreorder
.text
    LW $t0, a
    LW $t1, c
    LW $t2, res
    ADD $t2, $t0, $t1
.data
a:
    .word 5
c:
    .word 07
res:
    .word 0
