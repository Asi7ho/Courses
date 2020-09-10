  1                   # TEST_RETURN_CODE=PASS
  2                   # allons au ru
  3
  4
  5                   .set noreorder
  6                   .text
  7 00000000 3C010000 Lw $t0, lunchtime
  7 00000004 8C280000
  8 00000008 20090008 ADDI $t1,$zero,8
  9
 10                   boucle:
 11 0000000C 11090008 BEQ $t0 , $t1 , byebye
 12 00000010 00000000 NOP
 13 00000014 21290001 addi $t1 , $t1 , 1
 14 00000018 08600000 J boucle
 15 0000001C 00000000 NOP
 16                   byebye:
 17 00000020 0D000000 JAL byebye
 18
 19                   .data
 20                       lunchtime:
 21 00000000 0000000C         .word 12
 22 00000004 00000000         .word menu
 23
 23                   .bss
 24                       menu:
 26 00000000 00000018         .space 24

.symtab
 10    .text:0000000C boucle
 16    .text:00000020 byebye
 20    .data:00000000 lunchtime
 25    .bss:00000000 menu

rel.text
00000000        R_MIPS_HI16        .data:00000000  lunchtime
00000004        R_MIPS_LO16        .data:00000000  lunchtime
0000000C        R_MIPS_16        .text:00000020  byebye
00000018        R_MIPS_26        .text:0000000C  boucle
00000020        R_MIPS_26        .text:00000020  byebye


rel.data
00000004        R_MIPS_32        .bss:00000000  menu

