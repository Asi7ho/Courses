  1                   #TEST_RETURN_CODE=PASS
  2                   #test
  3                   .set noreorder
  4                   .text
  5                   a:
  6 00000000 012A4020 ADD $t0, $t1, $t2
  7                   b:
  8 00000004 21280004 ADDI $t0, $t1, 4
  9                   .data
 10 00000000 0000000C .word 12
 11 00000004 00000002 .byte 2
 12 00000005 61616122 .asciiz "aaa"
 13                   c:
 14 0000000B 0000002D .space 45
 14                   .bss
 15                   d:
 17 00000000 00000003 .space 3
 17                   e:

.symtab
  5    .text:00000000 a
  7    .text:00000004 b
 13    .data:0000000B c
 16    .bss:00000000 d
 18    .bss:00000004 e

rel.text

rel.data
