  1                   #TEST_RETURN_CODE=PASS
  2                   .set noreorder
  3                   .data
  4                   mips:
  5 00000000 00000007 .word 7
  6 00000004 73616C75 .asciiz "salut"
  7 00000008 74000000

 .symtab
  4    .data:00000000 mips

 rel.data
