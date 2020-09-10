  1                   #TEST_RETURN_CODE=FAIL
  2                   .text
  3                   boucle:
  4 00000000 20620022 ADDI $2, $3, 34
  5 00000004 08000000 J $23 

.symtab
  3    .text:00000000 boucle

rel.text

rel.data
