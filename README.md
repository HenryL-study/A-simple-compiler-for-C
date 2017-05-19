# A-simple-compiler-for-C
A compiler can convert C code to MIPS assemble code.

- C grammer see `C grammar.pdf `
- Target structure: `MIPS`
- Use `mars 4.5  ` for excuting MIPS code


Code example:

```assembly
.data
a: .space 40
.String0: .asciiz  " "
.text
lui $gp 0x1001
j main
quicksort: 
move $s4 $a0
move $t0 $s4
addi $k1 $gp 0
mul $t0 $t0 4
add $k1 $k1 $t0
lw $t1 0($k1)
lw $s1 -8($sp)
move $s1 $t1
sw $s1 -8($sp)
lw $s2 -0($sp)
move $s2 $s4
sw $s2 -0($sp)
move $s5 $a1
move $t2 $s5
li $t3 1
sub $t2 $t2 $t3
lw $s3 -4($sp)
move $s3 $t2
sw $s3 -4($sp)
move $t4 $s5
move $t5 $s4
sub $t4 $t4 $t5
move $t6 $t4
li $t7 1
sw $s2 -0($sp)
sw $s3 -4($sp)
move  $a1 $s5
move  $a0 $s4
sw $s1 -8($sp)
sw $t0 -16($sp)
sw $t6 -20($sp)
sw $t7 -24($sp)
sw $t1 -28($sp)
sw $t2 -32($sp)
sw $t3 -36($sp)
sw $t4 -40($sp)
sw $t5 -44($sp)
lw $t8 -20($sp)
lw $t9 -24($sp)
ble $t8 $t9 .label1
lw $s2 -0($sp)
lw $s0 -12($sp)
move $s0 $s2
sw $s0 -12($sp)
sw $s2 -0($sp)
sw $s0 -12($sp)
sw $t8 -20($sp)
sw $t9 -24($sp)
.label2: 
lw $s2 -0($sp)
move $t0 $s2
lw $s3 -4($sp)
move $t1 $s3
sw $s2 -0($sp)
sw $s3 -4($sp)
sw $t0 -48($sp)
sw $t1 -52($sp)
lw $t2 -48($sp)
lw $t3 -52($sp)
bge $t2 $t3 .label3
sw $t2 -48($sp)
sw $t3 -52($sp)
.label4: 
lw $s3 -4($sp)
move $t4 $s3
lw $s2 -0($sp)
move $t5 $s2
sw $s2 -0($sp)
sw $s3 -4($sp)
sw $t5 -56($sp)
sw $t4 -60($sp)
lw $t6 -60($sp)
lw $t7 -56($sp)
ble $t6 $t7 .label5
lw $s3 -4($sp)
move $t8 $s3
addi $k1 $gp 0
mul $t8 $t8 4
add $k1 $k1 $t8
lw $t9 0($k1)
move $t0 $t9
lw $s1 -8($sp)
move $t1 $s1
sw $s3 -4($sp)
sw $s1 -8($sp)
sw $t7 -56($sp)
sw $t0 -64($sp)
sw $t8 -68($sp)
sw $t1 -72($sp)
sw $t6 -60($sp)
sw $t9 -76($sp)
lw $t2 -64($sp)
lw $t3 -72($sp)
bge $t2 $t3 .label7
lw $s2 -0($sp)
move $t4 $s2
lw $s3 -4($sp)
move $t5 $s3
addi $k1 $gp 0
mul $t5 $t5 4
add $k1 $k1 $t5
lw $t6 0($k1)
move $t7 $t6
addi $k1 $gp 0
mul $t4 $t4 4
add $k1 $k1 $t4
sw $t7 0($k1)
move $t8 $s2
li $t9 1
add $t8 $t8 $t9
move $s2 $t8
sw $s2 -0($sp)
lw $s0 -12($sp)
move $s0 $s3
sw $s0 -12($sp)
move $t0 $s3
li $t1 20
sub $t0 $t0 $t1
move $s3 $t0
sw $s3 -4($sp)
sw $s2 -0($sp)
sw $s3 -4($sp)
sw $s0 -12($sp)
sw $t2 -64($sp)
sw $t3 -72($sp)
sw $t4 -80($sp)
sw $t7 -84($sp)
sw $t5 -88($sp)
sw $t6 -92($sp)
sw $t8 -96($sp)
sw $t9 -100($sp)
sw $t0 -104($sp)
sw $t1 -108($sp)
.label7: 
lw $s3 -4($sp)
move $t2 $s3
li $t3 1
sub $t2 $t2 $t3
move $s3 $t2
sw $s3 -4($sp)
move $t4 $s3
li $t5 0
sw $s3 -4($sp)
sw $t4 -112($sp)
sw $t5 -116($sp)
sw $t2 -120($sp)
sw $t3 -124($sp)
lw $t6 -112($sp)
lw $t7 -116($sp)
blt $t6 $t7 .label9
lw $s3 -4($sp)
lw $s0 -12($sp)
move $s0 $s3
sw $s0 -12($sp)
sw $s3 -4($sp)
sw $s0 -12($sp)
sw $t6 -112($sp)
sw $t7 -116($sp)
.label9: 
j .label4
.label5: 
lw $s0 -12($sp)
lw $s3 -4($sp)
move $s3 $s0
sw $s3 -4($sp)
sw $s3 -4($sp)
sw $s0 -12($sp)
.label10: 
lw $s2 -0($sp)
move $t8 $s2
lw $s3 -4($sp)
move $t9 $s3
sw $s2 -0($sp)
sw $s3 -4($sp)
sw $t8 -128($sp)
sw $t9 -132($sp)
lw $t0 -128($sp)
lw $t1 -132($sp)
bge $t0 $t1 .label11
lw $s2 -0($sp)
move $t2 $s2
addi $k1 $gp 0
mul $t2 $t2 4
add $k1 $k1 $t2
lw $t3 0($k1)
move $t4 $t3
lw $s1 -8($sp)
move $t5 $s1
sw $s2 -0($sp)
sw $s1 -8($sp)
sw $t0 -128($sp)
sw $t1 -132($sp)
sw $t4 -136($sp)
sw $t2 -140($sp)
sw $t5 -144($sp)
sw $t3 -148($sp)
lw $t6 -136($sp)
lw $t7 -144($sp)
ble $t6 $t7 .label13
lw $s3 -4($sp)
move $t8 $s3
lw $s2 -0($sp)
move $t9 $s2
addi $k1 $gp 0
mul $t9 $t9 4
add $k1 $k1 $t9
lw $t0 0($k1)
move $t1 $t0
addi $k1 $gp 0
mul $t8 $t8 4
add $k1 $k1 $t8
sw $t1 0($k1)
lw $s0 -12($sp)
move $s0 $s2
sw $s0 -12($sp)
move $t2 $s2
li $t3 20
add $t2 $t2 $t3
move $s2 $t2
sw $s2 -0($sp)
sw $s2 -0($sp)
sw $s3 -4($sp)
sw $s0 -12($sp)
sw $t6 -136($sp)
sw $t7 -144($sp)
sw $t8 -152($sp)
sw $t1 -156($sp)
sw $t9 -160($sp)
sw $t0 -164($sp)
sw $t2 -168($sp)
sw $t3 -172($sp)
.label13: 
lw $s2 -0($sp)
move $t4 $s2
li $t5 1
add $t4 $t4 $t5
move $s2 $t4
sw $s2 -0($sp)
move $t6 $s2
li $t7 10
sw $s2 -0($sp)
sw $t6 -176($sp)
sw $t7 -180($sp)
sw $t4 -184($sp)
sw $t5 -188($sp)
lw $t8 -176($sp)
lw $t9 -180($sp)
bge $t8 $t9 .label15
lw $s2 -0($sp)
lw $s0 -12($sp)
move $s0 $s2
sw $s0 -12($sp)
sw $s2 -0($sp)
sw $s0 -12($sp)
sw $t8 -176($sp)
sw $t9 -180($sp)
.label15: 
j .label10
.label11: 
lw $s0 -12($sp)
lw $s2 -0($sp)
move $s2 $s0
sw $s2 -0($sp)
sw $s2 -0($sp)
sw $s0 -12($sp)
j .label2
.label3: 
lw $s2 -0($sp)
move $t0 $s2
lw $s1 -8($sp)
move $t1 $s1
addi $k1 $gp 0
mul $t0 $t0 4
add $k1 $k1 $t0
sw $t1 0($k1)
move $s4 $a0
move $t2 $s4
move $t3 $s2
addi $sp $sp -188
addi $sp $sp -72
sw  $8 68($sp)
sw  $9 64($sp)
sw  $10 60($sp)
sw  $11 56($sp)
sw  $12 52($sp)
sw  $13 48($sp)
sw  $14 44($sp)
sw  $15 40($sp)
sw  $16 36($sp)
sw  $17 32($sp)
sw  $18 28($sp)
sw  $19 24($sp)
sw  $20 20($sp)
sw  $21 16($sp)
sw  $22 12($sp)
sw  $23 8($sp)
sw  $24 4($sp)
sw  $25 0($sp)
addi $sp $sp -4
sw $a0 0($sp)
sw $a1 -4($sp)
sw $a2 -8($sp)
sw $a3 -12($sp)
sw $ra -16($sp)
addi $sp $sp -20
addi $sp $sp 284
move $a0 $t2
move $a1 $t3
addi $sp $sp -284
jal quicksort
addi $sp $sp 20
lw $a0 0($sp)
lw $a1 -4($sp)
lw $a2 -8($sp)
lw $a3 -12($sp)
lw $ra -16($sp)
addi $sp $sp 72
lw  $8 -0($sp)
lw  $9 -4($sp)
lw  $10 -8($sp)
lw  $11 -12($sp)
lw  $12 -16($sp)
lw  $13 -20($sp)
lw  $14 -24($sp)
lw  $15 -28($sp)
lw  $16 -32($sp)
lw  $17 -36($sp)
lw  $18 -40($sp)
lw  $19 -44($sp)
lw  $20 -48($sp)
lw  $21 -52($sp)
lw  $22 -56($sp)
lw  $23 -60($sp)
lw  $24 -64($sp)
lw  $25 -68($sp)
addi $sp $sp 192
move $t4 $s2
li $t5 1
add $t4 $t4 $t5
move $t6 $t4
move $s5 $a1
move $t7 $s5
addi $sp $sp -188
addi $sp $sp -72
sw  $8 68($sp)
sw  $9 64($sp)
sw  $10 60($sp)
sw  $11 56($sp)
sw  $12 52($sp)
sw  $13 48($sp)
sw  $14 44($sp)
sw  $15 40($sp)
sw  $16 36($sp)
sw  $17 32($sp)
sw  $18 28($sp)
sw  $19 24($sp)
sw  $20 20($sp)
sw  $21 16($sp)
sw  $22 12($sp)
sw  $23 8($sp)
sw  $24 4($sp)
sw  $25 0($sp)
addi $sp $sp -4
sw $a0 0($sp)
sw $a1 -4($sp)
sw $a2 -8($sp)
sw $a3 -12($sp)
sw $ra -16($sp)
addi $sp $sp -20
addi $sp $sp 284
move $a0 $t6
move $a1 $t7
addi $sp $sp -284
jal quicksort
addi $sp $sp 20
lw $a0 0($sp)
lw $a1 -4($sp)
lw $a2 -8($sp)
lw $a3 -12($sp)
lw $ra -16($sp)
addi $sp $sp 72
lw  $8 -0($sp)
lw  $9 -4($sp)
lw  $10 -8($sp)
lw  $11 -12($sp)
lw  $12 -16($sp)
lw  $13 -20($sp)
lw  $14 -24($sp)
lw  $15 -28($sp)
lw  $16 -32($sp)
lw  $17 -36($sp)
lw  $18 -40($sp)
lw  $19 -44($sp)
lw  $20 -48($sp)
lw  $21 -52($sp)
lw  $22 -56($sp)
lw  $23 -60($sp)
lw  $24 -64($sp)
lw  $25 -68($sp)
addi $sp $sp 192
sw $s2 -0($sp)
move  $a1 $s5
move  $a0 $s4
sw $s1 -8($sp)
sw $t0 -192($sp)
sw $t1 -196($sp)
sw $t2 -200($sp)
sw $t3 -204($sp)
sw $t6 -208($sp)
sw $t7 -212($sp)
sw $t4 -216($sp)
sw $t5 -220($sp)
.label1: 
jr $ra
jr $ra
main: 
lw $s0 -0($sp)
li $s0 0
sw $s0 -0($sp)
sw $s0 -0($sp)
.label16: 
lw $s0 -0($sp)
move $t0 $s0
li $t1 10
sw $s0 -0($sp)
sw $t0 -12($sp)
sw $t1 -16($sp)
lw $t2 -12($sp)
lw $t3 -16($sp)
bge $t2 $t3 .label17
li $v0 5 
syscall
lw $s3 -8($sp)
move $s3 $v0
lw $s0 -0($sp)
move $t4 $s0
move $t5 $s3
addi $k1 $gp 0
mul $t4 $t4 4
add $k1 $k1 $t4
sw $t5 0($k1)
move $t6 $s0
li $t7 1
add $t6 $t6 $t7
move $s0 $t6
sw $s0 -0($sp)
sw $s0 -0($sp)
sw $s3 -8($sp)
sw $t2 -12($sp)
sw $t3 -16($sp)
sw $t4 -20($sp)
sw $t5 -24($sp)
sw $t6 -28($sp)
sw $t7 -32($sp)
j .label16
.label17: 
lw $s2 -4($sp)
li $s2 0
sw $s2 -4($sp)
move $t8 $s2
li $t9 10
addi $sp $sp -32
addi $sp $sp -72
sw  $8 68($sp)
sw  $9 64($sp)
sw  $10 60($sp)
sw  $11 56($sp)
sw  $12 52($sp)
sw  $13 48($sp)
sw  $14 44($sp)
sw  $15 40($sp)
sw  $16 36($sp)
sw  $17 32($sp)
sw  $18 28($sp)
sw  $19 24($sp)
sw  $20 20($sp)
sw  $21 16($sp)
sw  $22 12($sp)
sw  $23 8($sp)
sw  $24 4($sp)
sw  $25 0($sp)
addi $sp $sp -4
sw $a0 0($sp)
sw $a1 -4($sp)
sw $a2 -8($sp)
sw $a3 -12($sp)
sw $ra -16($sp)
addi $sp $sp -20
addi $sp $sp 128
move $a0 $t8
move $a1 $t9
addi $sp $sp -128
jal quicksort
addi $sp $sp 20
lw $a0 0($sp)
lw $a1 -4($sp)
lw $a2 -8($sp)
lw $a3 -12($sp)
lw $ra -16($sp)
addi $sp $sp 72
lw  $8 -0($sp)
lw  $9 -4($sp)
lw  $10 -8($sp)
lw  $11 -12($sp)
lw  $12 -16($sp)
lw  $13 -20($sp)
lw  $14 -24($sp)
lw  $15 -28($sp)
lw  $16 -32($sp)
lw  $17 -36($sp)
lw  $18 -40($sp)
lw  $19 -44($sp)
lw  $20 -48($sp)
lw  $21 -52($sp)
lw  $22 -56($sp)
lw  $23 -60($sp)
lw  $24 -64($sp)
lw  $25 -68($sp)
addi $sp $sp 36
lw $s0 -0($sp)
li $s0 0
sw $s0 -0($sp)
sw $s0 -0($sp)
sw $s2 -4($sp)
sw $t8 -36($sp)
sw $t9 -40($sp)
.label18: 
lw $s0 -0($sp)
move $t0 $s0
li $t1 10
sw $s0 -0($sp)
sw $t0 -44($sp)
sw $t1 -48($sp)
lw $t2 -44($sp)
lw $t3 -48($sp)
bge $t2 $t3 .label19
lw $s0 -0($sp)
move $t4 $s0
addi $k1 $gp 0
mul $t4 $t4 4
add $k1 $k1 $t4
lw $t5 0($k1)
move $t6 $t5
move $k0 $a0
li $v0 1 
move $a0 $t6 
syscall
move $a0 $k0
move $k0 $a0
la $a0 .String0
li $v0 4 
syscall
move $a0 $k0
move $t7 $s0
li $t8 1
add $t7 $t7 $t8
move $s0 $t7
sw $s0 -0($sp)
sw $s0 -0($sp)
sw $t2 -44($sp)
sw $t3 -48($sp)
sw $t6 -52($sp)
sw $t4 -56($sp)
sw $t5 -60($sp)
sw $t7 -64($sp)
sw $t8 -68($sp)
j .label18
.label19: 
j main_end
main_end:
```