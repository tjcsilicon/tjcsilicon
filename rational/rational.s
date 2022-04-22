
# This file will contain all of your rat_*() functions (and nothing
# else). As an example, we've included rat_print() below.

    .option pic0
    
    .rdata # read-only data
    
    .align  2
resultFormat:
    .asciz  "(%d/%u)\n"
    
    .text
    .align  2

    # You need to put a ".global" assembler directive for every
    # function you expect to call from outside this file. (Why doesn't
    # "resultFormat" need a ".global"?
    .global rat_print

rat_print:
    # ($a0, $a1) -> () (no output registers)
    # prints "(i/j)" on stdout, where i = $a0 and j = $a1
    addiu $sp, $sp, -4  # create space on the stack for $sp (why?)
    sw    $ra, ($sp)    # save $sp on the stack (why?)
    
    # call printf() with the appropriate format and arguments
    # (Why not do it in $a0, $a1, $a2 order?)
    move $a2, $a1    
    move $a1, $a0
    la   $a0, resultFormat
    jal  printf
    
    lw    $ra, ($sp)    # restore $sp from the stack (why?)
    addiu $sp, $sp, +4  # release the stack space used for $sp

    jr    $ra

# put your additional rat_* functions here
rat_mul:

    .global rat_mul

    addiu $sp, $sp, -4 #save return address on stack
    sw    $ra, ($sp)

    mult  $a0, $a2 #multiplies both numerators
    mfhi  $t0 #transfer upper half
    mflo  $t1 #transfer lower half
    or    $a0, $t1, $t0 #combine top and bottom to a single 32 bit number
    
    mult  $a1, $a3 #multiplies both denominators
    mfhi  $t0 #transfer upper half
    mflo  $t1 #transfer lower half
    or    $a1, $t1, $t0 #combine top and bottom to a single 32 bit number
    
    jal rat_simplify

    lw    $ra, ($sp) #replaces return address from stack
    addiu $sp, $sp, 4


    jr    $ra
    
rat_div:
    
    .global rat_div 

    addiu $sp, $sp, -4 #save return address on stack
    sw    $ra, ($sp)
    
    move  $t2, $a2 #saves top and bottom of second rat number
    move  $t3, $a3

    move  $a2, $t3 #makes second rat the inverse
    move  $a3, $t2

    jal rat_mul #multiplies first rat number by the inverse of second completing rat division
    
    move $a0, $v0
    move $a1, $v1

    jal rat_simplify

    lw    $ra, ($sp) #replaces return address from stack
    addiu $sp, $sp, 4

    jr    $ra

rat_add:
    
    .global rat_add

    addiu $sp, $sp, -4
    sw    $ra, ($sp)
    
    mult  $a0, $a3
    mfhi  $t4
    mflo  $t5
    or    $t0, $t4, $t5

    mult  $a1, $a3
    mfhi  $t4
    mflo  $t5
    or    $t1, $t4, $t5

    mult  $a2, $a1
    mfhi  $t4
    mflo  $t5
    or    $t2, $t4, $t5

    mult  $a3, $t1
    mfhi  $t4
    mflo  $t5
    or    $t3, $t4, $t5

 
    add   $a0, $t0, $t2 #adds the numerators
    move  $a1, $t1
    
    jal rat_simplify

    lw    $ra, ($sp) #replaces return address from stack
    addiu $sp, $sp, 4

    jr $ra

rat_sub:
   
    .global rat_sub

    addiu $sp, $sp, -4
    sw    $ra, ($sp)
    
    mult  $a0, $a3
    mfhi  $t4
    mflo  $t5
    or    $t0, $t4, $t5

    mult  $a1, $a3
    mfhi  $t4
    mflo  $t5
    or    $t1, $t4, $t5

    mult  $a2, $a1
    mfhi  $t4
    mflo  $t5
    or    $t2, $t4, $t5

    mult  $a3, $a1
    mfhi  $t4
    mflo  $t5
    or    $t3, $t4, $t5

 
    sub   $a0, $t0, $t2 #adds the numerators
    move  $a1, $t1
    
    jal rat_simplify

    lw    $ra, ($sp) #replaces return address from stack
    addiu $sp, $sp, 4

    jr $ra

rat_simplify:
    
    .global rat_simplify
    
    addiu $sp, $sp, -4 #store return address on stack
    sw    $ra, ($sp)

    move  $t0, $a0 #preserves the value of arguments
    move  $t1, $a1

    jal gcd #finds GCD
    move  $t2, $v0 #store GCD in $t2
    
    div   $a0, $t2 #divide top by GCD
    mflo  $v0 #take result of div from lo
    div   $a1, $t2 #divide bottom by GCD
    mflo  $v1 #take result of div from lo 

    lw    $ra, ($sp) #replaces return address from stack
    addiu $sp, $sp, 4

    jr $ra
