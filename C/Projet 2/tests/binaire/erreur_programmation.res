 1                   #TEST_RETURN_CODE=FAIL
 2                   #erreur de programmation
 3
 4                   .set noreorder
 5                   .text
 6 00000000 3C010000   LW $t0, a
 7 00000004 8D010000
 8 00000008 3C010000   LW $t1, b
 9 0000000C 8E210004
10 00000010 3C010000   LW $t2, res
11 00000014 8E310008
12 00000018 00044020   ADD res, a, b
13                   .data
14                   a:
15 00000000 00000005 .word 5
16                   b:
17 00000004 00000007 .word 07
18                   res:
19 00000008 00000000 .word 0

.symtab
14    .data:00000000 a
16    .data:00000004 b
18    .data:00000008 res

rel.text
00000000        R_MIPS_26	 .data:00000000	a
00000008        R_MIPS_26	 .data:00000004	b
00000010        R_MIPS_26	 .data:00000008	res
00000018        R_MIPS_26	 .data:00000000	a
00000018        R_MIPS_26	 .data:00000004	b
00000018        R_MIPS_26	 .data:00000008	res

rel.data
