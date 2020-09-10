#TEST_RETURN_CODE=FAIL
# erreur texte

.set noreorder
.text
LW $t0, a-
LW $t1, b/
LW $33, res
ADD re\s, a, b
.data
a-:
.word 5
b/:
.word 07
re\s:
.word 0
