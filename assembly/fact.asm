fact:
sw $ra, 32($sp)
sw $s0, ($sp)
sw $s1, 4($sp)
sw $s2, 8($sp)
sw $s3, 12($sp)
sw $s4, 16($sp)
sw $s5, 20($sp)
sw $s6, 24($sp)
sw $s7, 28($sp)
.BB_fact_1:
lw $s0, 40($sp)
beq $s0, 0, .BB_fact_2
sw $s0, 40($sp)
.BB_fact_3:
lw $s0, 36($sp)
lw $s1, 40($sp)
sub $s0, $s1, 1
sw $s0, -4($sp)
sub $sp, $sp, 44
jal fact
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
sw $s0, 36($sp)
sw $s1, 40($sp)
b .BB_fact_4
.BB_fact_2:
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
.BB_fact_4:
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