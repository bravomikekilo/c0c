.data
        .global_arr: .space 400
        # ----- string section -----
        .str_0: .asciiz "sum is:"
        # ----- global section -----
       

.text
jal main
li $v0, 10
syscall
sum:
sw $ra, 32($sp)
sw $s0, ($sp)
sw $s1, 4($sp)
sw $s2, 8($sp)
sw $s3, 12($sp)
sw $s4, 16($sp)
sw $s5, 20($sp)
sw $s6, 24($sp)
sw $s7, 28($sp)
.BB_sum_1:
lw $s0, 44($sp)
li $s0, 0
sw $s0, 44($sp)
.BB_sum_2:
lw $s1, 48($sp)
lw $s0, 44($sp)
blt $s0, $s1, .BB_sum_3
sw $s1, 48($sp)
sw $s0, 44($sp)
.BB_sum_4:
lw $s1, 44($sp)
lw $s0, 40($sp)
li $s0, 0
li $s1, 0
sw $s1, 44($sp)
sw $s0, 40($sp)
b .BB_sum_5
.BB_sum_3:
lw $s1, 44($sp)
lw $s0, 36($sp)
add $s0, $s1, 1
la $t0, .global_arr
move $t1, $s1
add $t1, $t0, $t1
move $t0, $s0
sb $t0, ($t1)
add $s1, $s1, 1
sw $s1, 44($sp)
sw $s0, 36($sp)
b .BB_sum_2
.BB_sum_5:
lw $s1, 48($sp)
lw $s0, 44($sp)
blt $s0, $s1, .BB_sum_6
sw $s1, 48($sp)
sw $s0, 44($sp)
.BB_sum_7:
lw $s0, 40($sp)
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
add $sp, $sp, 52
jr $ra
sw $s0, 40($sp)
.BB_sum_6:
lw $s2, 40($sp)
lw $s0, 36($sp)
lw $s1, 44($sp)
la $t0, .global_arr
move $t1, $s1
mul $t1, $t1, 4
add $t1, $t1, $t0
lw $s0, ($t1)
add $s2, $s2, $s0
add $s1, $s1, 1
sw $s2, 40($sp)
sw $s0, 36($sp)
sw $s1, 44($sp)
b .BB_sum_5
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
.BB_main_8:
lw $s0, 36($sp)
li $t0, 10
sw $t0, -4($sp)
sub $sp, $sp, 52
jal sum
move $s0, $v0
li $v0, 4
la $a0, .str_0
syscall
li $v0, 1
move $a0, $s0
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
add $sp, $sp, 40
jr $ra
sw $s0, 36($sp)
