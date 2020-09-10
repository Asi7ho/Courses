  1                   #TEST_RETURN_CODE=PASS
  2                   # compteur
  3
  4                   .set noreorder
  5                   .text
  6 00000000 3C010000 LW $t0, compteur
  6 00000004 8C280000
  7 00000008 3C010000 LW $t1, fin_compteur
  7 0000000C 8C290004
  8                   boucle:
  9 00000010 11090009 BEQ $t0, $t1, end
 10 00000014 00000000 NOP
 11 00000018 21080001 ADDI $t0, $t0, 1
 12 0000001C 08800000 J boucle
 13 00000020 00000000 NOP
 14                   end:
 15 00000024 09200000 J end
 16                   .data
 17                   compteur:
 18 00000000 00000001 .word 1
 19                   fin_compteur:
 20 00000004 00000064 .word 100

.symtab
  8    .text:00000010 boucle
 14    .text:00000024 end
 17    .data:00000000 compteur
 19    .data:00000004 fin_compteur

rel.text
00000000        R_MIPS_HI16      .data:00000000  compteur
00000004        R_MIPS_LO16      .data:00000000  compteur
00000008        R_MIPS_HI16      .data:00000004  fin_compteur
0000000C        R_MIPS_LO16      .data:00000004  fin_compteur
00000010        R_MIPS_16        .text:00000024  end
0000001C        R_MIPS_26        .text:00000010  boucle
00000024        R_MIPS_26        .text:00000024  end


rel.data
