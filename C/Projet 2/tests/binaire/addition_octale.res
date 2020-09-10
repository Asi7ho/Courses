  1                   #TEST_RETURN_CODE=PASS
  2                   # addition octale
  3
  4                   .set noreorder
  5                   .text
  6 00000000 3C010000     LW $t0, a
  6 00000004 8C280000
  7 00000008 3C010000     LW $t1, c
  7 0000000C 8C290004
  8 00000010 3C010000     LW $t2, res
  8 00000014 8C2A0008
  9 00000018 01095020     ADD $t2, $t0, $t1
 10                   .data
 11                   a:
 12 00000000 00000005     .word 5
 13                   c:
 14 00000004 00000007     .word 07
 15                   res:
 16 00000008 00000000     .word 0

.symtab
 11    .data:00000000 a
 13    .data:00000004 c
 15    .data:00000008 res

rel.text
00000000        R_MIPS_HI16        .data:00000000  a
00000004        R_MIPS_LO16        .data:00000000  a
00000008        R_MIPS_HI16        .data:00000004  c
0000000C        R_MIPS_LO16        .data:00000004  c
00000010        R_MIPS_HI16        .data:00000008  res
00000014        R_MIPS_LO16        .data:00000008  res


rel.data
