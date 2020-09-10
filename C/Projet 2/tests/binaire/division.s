#TEST_RETURN_CODE=PASS
# division

.set noreorder
.text
#initialise les variables
ADDI  $t9,$zero,EXIT
ADDI  $t2,$zero,-	43
ADDI  $t3,	$zero,0xffff

#fait la division
DIV  $t2,$t3
MFLO $t0
MFHI $t1
BEQ $t1,$zero, 60
ADD $t2,$zero,$zero
J EXIT

EXIT	:
    J EXIT
## The End

.data
.byte 12,0xAA,0xBB,0xCC,0xdd
.byte 0xFF
.word 0xAABBCCDD
