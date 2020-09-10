  1                   #TEST_RETURN_CODE=PASS
  2                   .text
  3                   boucle:
  4 00000000 00641020   ADD $2, $3, $4
  5                   test:
  6 00000004 20620022   ADDI $2, $3, 34
  7 00000008 00000004   J test
  8                   .data
  9 00000000 00000000 word boucle

.symtab
 3    .text:00000000  boucle
 5    .text:00000004  test

 rel.text
 00000008	    R_MIPS_26	      .text:00000004	test

 rel.data
 00000000     R_MIPS_26       .text:00000000 boucle
