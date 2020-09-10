#TEST_RETURN_CODE=PASS
.set noreorder
.text
    ADDI $2, $3, 1
test:
    ADDI $2, $3, 34
    J test
