.data
	 str_hello_world_str: .asciiz "hello world\n" 
.text
j main


add:
sw $ra, 32($sp)
sw $s0, ($sp)
sw $s1, 4($sp)
sw $s2, 8($sp)
sw $s3, 12($sp)
sw $s4, 16($sp)
sw $s5, 20($sp)
sw $s6, 24($sp)
sw $s7, 28($sp)
.BB_add1:
lw $s0, 36($sp)
lw $s1, 44($sp)
lw $s2, 40($sp)
add $s0, $s1, $s2
lw $ra, 32($sp)
move $v0 $s0
lw $s0, ($sp)
lw $s1, 4($sp)
lw $s2, 8($sp)
lw $s3, 12($sp)
lw $s4, 16($sp)
lw $s5, 20($sp)
lw $s6, 24($sp)
lw $s7, 28($sp)
sub $sp, $sp, 48
jr $ra
sw $s0, 36
sw $s1, 44
sw $s2, 40

main:
	# print a hello world
	li $v0, 4 
	la $a0, str_hello_world_str
	syscall
	
	li $t0 1
	li $t1 2
	sw $t0 -4($sp)
	sw $t1 -8($sp)
	
	sub $sp $sp 48
	
	jal add
	
	
	
	# print result of function
	move $a0, $v0
	li $v0, 1
	syscall

