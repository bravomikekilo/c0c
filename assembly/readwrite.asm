.data
        # ----- string section -----
        .str_0: .asciiz "hello world"
        # ----- global section -----

.text
jal main
li $v0, 10
syscall
main:
sw $ra, 32($sp)
sw $s0, ($sp)
sw $s1, 4($sp)
sw $s2, 8($sp)
sw $s3, 12($sp)
sw $s4, 16($sp)
sw $s5, 20($sp)
sw $s6, 24($sp)
sw $s7, 28($sp)
.BB_main_1:
lw $s3, 44($sp)
lw $s4, 36($sp)
lw $s0, 52($sp)
lw $s2, 48($sp)
li $v0, 5
syscall
move $s0, $v0
li $v0, 5
syscall
move $s0, $v0
li $v0, 5
syscall
move $s2, $v0
li $v0, 5
syscall
move $s3, $v0
li $v0, 4
la $a0, .str_0
syscall
li $v0, 11
li $a0, 10
syscall
li $s4, 3
add $s4, $s4, 3
li $v0, 4
la $a0, .str_0
syscall
li $v0, 1
move $a0, $s4
syscall
li $v0, 11
li $a0, 10
syscall
li $s4, 3
add $s4, $s4, 3
li $v0, 1
move $a0, $s4
syscall
li $v0, 11
li $a0, 10
syscall
mul $s4, $s2, $s3
add $s4, $s0, $s4
li $v0, 1
move $a0, $s4
syscall
li $v0, 11
li $a0, 10
syscall
lw $ra, 32($sp)
lw $s0, ($sp)
lw $s1, 4($sp)
lw $s2, 8($sp)
lw $s3, 12($sp)
lw $s4, 16($sp)
lw $s5, 20($sp)
lw $s6, 24($sp)
lw $s7, 28($sp)
add $sp, $sp, 56
jr $ra
sw $s3, 44($sp)
sw $s4, 36($sp)
sw $s0, 52($sp)
sw $s2, 48($sp)

