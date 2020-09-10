  1                   #TEST_RETURN_CODE=PASS
  2                   .set noreorder
  3                   .text
  4 00000000 20430001     ADDI $2, $3, 1
  5                   test:
  6 00000004 20430022     ADDI $2, $3, 34
  7 00000008 08200000     J test
  8                       J test

.symtab
  5    .text:00000004 test

rel.text
00000008        R_MIPS_26        .text:00000004  test


rel.data
