  1                   #TEST_RETURN_CODE=PASS
  2                   # division
  3
  4                   .set noreorder
  5                   .text
  6                   #initialise les variables
  7 00000000 20190024 ADDI  $t9,$zero,EXIT
  8 00000004 200AFFD5 ADDI  $t2,$zero,-	43
  9 00000008 200BFFFF ADDI  $t3,	$zero,0xffff
 10
 11                   #fait la division
 12 0000000C 014B001A DIV  $t2,$t3
 13 00000010 00004012 MFLO $t0
 14 00000014 00004810 MFHI $t1
 15 00000018 1120000F BEQ $t1,$zero, 60
 16 0000001C 00005020 ADD $t2,$zero,$zero
 17 00000020 09200000 J EXIT
 18
 19                   EXIT	:
 20 00000024 09200000     J EXIT
 21                   ## The End
 22
 23                   .data
 24 00000000 0CAABBCC .byte 12,0xAA,0xBB,0xCC,0xdd
 24 00000004 000000DD
 25 00000005 000000FF .byte 0xFF
 26 00000006 AABBCCDD .word 0xAABBCCDD

.symtab
 19    .text:00000024 EXIT

rel.text
00000000        R_MIPS_16        .text:00000024  EXIT
00000020        R_MIPS_26        .text:00000024  EXIT
00000024        R_MIPS_26        .text:00000024  EXIT


rel.data
