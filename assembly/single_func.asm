

.data
	str_hello_world_str: .asciiz "hello world\n"

.text
j main

# please put your function here


main:
	# print a hello world
	li $v0, 4
	la $a0, str_hello_world_str
	syscall

	# set up your function argument
	# load argument into temp register
	li $t0, 1
	li $t1, 2
	
	# store your argument into stack
	sw $t0, -4($sp)
	sw $t1, -8($sp)
	
	# move your frame pointer
	sub $sp, $sp, 48
	
	# call your function
	jal add

	# print result of function
	move $a0, $v0
	li $v0, 1
	syscall

