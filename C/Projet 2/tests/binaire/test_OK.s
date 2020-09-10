#TEST_RETURN_CODE=PASS
#test
.set noreorder
.text
a:
ADD $t0, $t1, $t2
b:
ADDI $t0, $t1, 4
.data
.word 12
.byte 2
.asciiz "aaa"
c:
.space 45
.bss
d:
.space 3
e:
