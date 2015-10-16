#################################################################################
#	File : reset.s
#	Author : Alain Greiner
#	Date : 28/11/2013
#################################################################################
#	- It initializes the Interrupt vector
#	- It initializes the Status Register (SR) 
#	- It defines the stack size  and initializes the stack pointer ($29) 
#	- It initializes the EPC register, and jump to the user code    
#################################################################################
		
	.section .reset,"ax",@progbits

	.extern	seg_stack_base
	.extern	seg_data_base
	.extern	seg_icu_base
	.extern _interrupt_vector
	.extern _isr_timer
	.extern _isr_tty_get_task0
	.extern _isr_dma
	.extern _isr_ioc

	.globl  reset	 			# makes reset an external symbol 
	.ent	reset
	.align	2

reset:
       	.set noreorder

# initializes stack pointer
    mfc0    $26,    $15,    1             # procid 
    andi    $26,    $26,    0xf
    sll     $26,    $26,    16
    addu    $26,    $26,    0x10000
    la      $27,    seg_stack_base
    addu    $29,    $26,    $27 	# $29 <= seg_stack_base + 64K

# initializes interrupt vector
    la      $26,    _interrupt_vector   # interrupt vector address
    la      $27,    _isr_ioc           
    sw      $27,    0($26)              # interrupt_vector[0] <= _isr_ioc
    la      $27,    _isr_dma             
    sw      $27,    16($26)             # interrupt_vector[4] <= _isr_dma
    la      $27,    _isr_dma             
    sw      $27,    20($26)             # interrupt_vector[5] <= _isr_dma
    la      $27,    _isr_dma             
    sw      $27,    24($26)             # interrupt_vector[6] <= _isr_dma
    la      $27,    _isr_dma             
    sw      $27,    28($26)             # interrupt_vector[7] <= _isr_dma
    la      $27,    _isr_timer             
    sw      $27,    32($26)             # interrupt_vector[8] <= _isr_timer
    la      $27,    _isr_timer             
    sw      $27,    36($26)             # interrupt_vector[9] <= _isr_timer
    la      $27,    _isr_timer             
    sw      $27,    40($26)             # interrupt_vector[10] <= _isr_timer
    la      $27,    _isr_timer             
    sw      $27,    44($26)             # interrupt_vector[11] <= _isr_timer
    la      $27,    _isr_tty_get_task0              
    sw      $27,    48($26)              # interrupt_vector[12] <= _isr_tty_get_task0
    la      $27,    _isr_tty_get_task0              
    sw      $27,    52($26)              # interrupt_vector[13] <= _isr_tty_get_task0
    la      $27,    _isr_tty_get_task0              
    sw      $27,    56($26)              # interrupt_vector[14] <= _isr_tty_get_task0
    la      $27,    _isr_tty_get_task0              
    sw      $27,    60($26)              # interrupt_vector[15] <= _isr_tty_get_task0

# initializes ICU
    mfc0    $26,    $15,   1                 # procid 
    andi    $26,    $26,    0xf
    li      $27,    0x00001111          # IRQ[0] IRQ[1] IRQ[2] IRQ[3]
    sllv    $27,    $27,    $26

    sll     $26,    $26,   5
    la      $25,    seg_icu_base
    addu    $26,    $26,   $25
    sw      $27,    8($26)             # ICU_MASK_SET

# initializes SR register
    li	    $26,    0x0000FF13		
    mtc0    $26,    $12			# SR <= user mode / IRQ enable (after eret)

# jumps to main 
#    la      $26,    seg_data_base
#    lw      $26,    0($26)
#    mtc0    $26,    $14


    mfc0    $27,    $15, 1                 # procid 
    andi    $26,    $26,    0xf
    sll     $27,    $27, 2
    la      $26,    seg_data_base
    addu    $26,    $27,   $26
    lw      $26,    0($26)
    mtc0    $26,    $14
    eret

	.end	reset
	.size	reset, .-reset

	.set reorder
