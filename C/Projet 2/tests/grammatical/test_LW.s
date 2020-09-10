#test LW

.set noreorder
.text
boucle:
    LW $t0, 200($t1)
.bss
    .space 32
