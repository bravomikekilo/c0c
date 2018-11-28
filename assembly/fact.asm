.data
        # ----- string section -----
        .str_0: .asciiz "called"
        # ----- global section -----

.text
jal main
li $v0 10
syscall
factWithPrint:
sw $ra, 32($sp)
sw $s0, ($sp)
sw $s1, 4($sp)
sw $s2, 8($sp)
sw $s3, 12($sp)
sw $s4, 16($sp)
sw $s5, 20($sp)
sw $s6, 24($sp)
sw $s7, 28($sp)
.BB_factWithPrint_1:
lw $s2, 44($sp)
lw $s0, 36($sp)
lw $s1, 40($sp)
mul $s0, $s1, 2
li $v0, 4
la $a0, .str_0
syscall
li $v0, 1
move $a0, $s0
syscall
li $v0, 11
li $a0, 10
syscall
beq $s1, 0, .BB_factWithPrint_2
sw $s2, 44($sp)
sw $s0, 36($sp)
sw $s1, 40($sp)
.BB_factWithPrint_3:
lw $s1, 40($sp)
lw $s0, 36($sp)
sub $s0, $s1, 1
sw $s0, -4($sp)
sub $sp, $sp, 44
jal factWithPrint
move $s0, $v0
mul $s0, $s1, $s0
lw $ra, 32($sp)
move $v0, $s0
lw $s0, ($sp)
lw $s1, 4($sp)
lw $s2, 8($sp)
lw $s3, 12($sp)
lw $s4, 16($sp)
lw $s5, 20($sp)
lw $s6, 24($sp)
lw $s7, 28($sp)
add $sp, $sp, 44
jr $ra
sw $s1, 40($sp)
sw $s0, 36($sp)
b .BB_factWithPrint_4
.BB_factWithPrint_2:
lw $ra, 32($sp)
li $v0, 1
lw $s0, ($sp)
lw $s1, 4($sp)
lw $s2, 8($sp)
lw $s3, 12($sp)
lw $s4, 16($sp)
lw $s5, 20($sp)
lw $s6, 24($sp)
lw $s7, 28($sp)
add $sp, $sp, 44
jr $ra
.BB_factWithPrint_4:
lw $ra, 32($sp)
li $v0, 1
lw $s0, ($sp)
lw $s1, 4($sp)
lw $s2, 8($sp)
lw $s3, 12($sp)
lw $s4, 16($sp)
lw $s5, 20($sp)
lw $s6, 24($sp)
lw $s7, 28($sp)
add $sp, $sp, 44
jr $ra
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
.BB_main_5:
lw $s1, 36($sp)
lw $s0, 40($sp)
li $v0, 5
syscall
move $s0, $v0
sw $s0, -4($sp)
sub $sp, $sp, 44
jal factWithPrint
move $s1, $v0
li $v0, 1
move $a0, $s1
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
add $sp, $sp, 44
jr $ra
sw $s1, 36($sp)
sw $s0, 40($sp)