 1                   #TEST_RETURN_CODE=FAIL
 2                   # erreur texte
 3
 4                   .set noreorder
 5                   .text
 6 00000000 00000000   LW $t0, a-
 7 00000008 00000000   LW $t1, b/
 8 00000012 00000000   LW $33, res
 9 00000010 00000000   ADD re\s, a, b
10                   .data
11                   a-:
12 00000000 00000005 .word 5
13                   b/:
14 00000004 00000007 .word 07
15                   re\s:
16 00000008 00000000 .word 0

.symtab

rel.text
rel.data
