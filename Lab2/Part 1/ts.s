	// ts.s file
	
	.global start
start:
    LDR sp, =stack_top
    BL main
    B .