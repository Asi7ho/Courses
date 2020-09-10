  1                   #TEST_RETURN_CODE=PASS
  2                   #test
  3                   .set noreorder
  4                   .data
  5 00000000 0000000C .word 12
  6 00000004 00000002 .byte 2
  7 00000005 61616122 .asciiz "aaa"
  8 0000000B 0000002D .space 45
  8                   .bss
 10 00000000 00000003 .space 3

.symtab

rel.text

rel.data
