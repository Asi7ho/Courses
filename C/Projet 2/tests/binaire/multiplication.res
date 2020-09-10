  1                   #TEST_RETURN_CODE=PASS
  2                   # multiplication
  3                   .set noreorder
  4                   .text
  5
  6                   ##----------------------------------------------------------------------------
  7                   # initialise les variables
  8 00000000 2019003C   ADDI  $t9,$zero,EXIT
  9 00000004 200AFFD5   ADDI  $t2,$zero,-	43
 10 00000008 200BFFFF   ADDI  $t3,	$0,0xffff
 11                   #fait la multiplication (remarquez le nom de l'Ã©tiquette)
 12                   mult:
 13 0000000C 00000000   NOP
 14 00000010 00000000   NOP
 15 00000014 010B0018   MULT $t0,$t3
 16 00000018 00004012   MFLO $t0
 17 0000001C 00004810   MFHI $t1
 18 00000020 00006820   ADD $t5,$zero,$zero
 19 00000024 152D000F   BNE $t1,$t5,EXIT
 20 00000028 110A000D   BEQ $t0,$t2,reussi
 21 0000002C 00005020   ADD $t2,$zero,$zero
 22 00000030 09E00000   J EXIT
 23                   reussi:
 24 00000034 200A0001   ADDI $t2,$zero,0x1
 25 00000038 09E00000   J EXIT
 26
 27                   EXIT	:
 28
 29                   ## The End
 30
 31                   .data
 32 00000000 0CAABBCC .byte 12,0xAA,0xBB,0xCC,0xDD
 32 00000004 000000DD
 33 00000005 000000FF .byte 0xFF
 34 00000006 AABBCCDD .word 0xAABBCCDD

.symtab
 12    .text:0000000C mult
 23    .text:00000034 reussi
 27    .text:0000003C EXIT

rel.text
00000000        R_MIPS_16        .text:0000003C  EXIT
00000024        R_MIPS_16        .text:0000003C  EXIT
00000028        R_MIPS_16        .text:00000034  reussi
00000030        R_MIPS_26        .text:0000003C  EXIT
00000038        R_MIPS_26        .text:0000003C  EXIT


rel.data
