  1                   #TEST_RETURN_CODE=PASS
  2
  3                   .data
  4 00000000 0000000C .word 12
  5 00000004 00000002 .byte 2
  6 00000005 61616122 .asciiz "aaa"
  7 0000000B 0000002D .space 45
  7                   .bss
  9 00000000 00000003 .space 3

.symtab

rel.text

rel.data
