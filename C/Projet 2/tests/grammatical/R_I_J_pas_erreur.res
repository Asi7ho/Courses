  1                   #TEST_RETURN_CODE=PASS
  2
  3                   .text
  4                   boucle:
  5 00000000 00432020   ADD $2, $3, $4
  6 00000004 0262002D   ADDI $2, $3, 45
  7 00000008 08000000   J boucle

.symtab
 4    .text:00000000 boucle

rel.text
00000008   R_MIPS_26	       .text:00000000  boucle

rel.data
