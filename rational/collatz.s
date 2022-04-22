    .option pic0

    .rdata

    .align 2

resultFormat:

    .asciz "%d\n"

    .text
    .align 2


    .global collatz_print

.data
    errorRange:  .asciiz "ERROR, NUMBER NOT VALID\n"
    .text


collatz_print:
    .global collatz_print

    addiu $sp, $sp, -20
    sw    $ra, ($sp)

    move  $a1, $a0  #printf takes resultFormat as first argument
    la    $a0, resultFormat
    jal   printf

    lw    $ra, ($sp)
    addiu $sp, $sp, 20

    jr    $ra

collatz_atoi:

    addiu $sp, $sp, -4
    sw    $ra, ($sp)
    
    jal   atoi #calls C function atoi to covert from char * to int

    move  $s0, $v0 #move output from atoi to $s0 for testing
    li    $t0, 0x55555555 #0x55555555 is 1431655765 which is 3n - 1 = 2^32 - 1 i 
    slt   $t0, $s0, $t0 #checks if $s0 is less than 0x55555555
    beq   $t0, $0, outofrange #branch if $t0 is 0
    addi  $t2, $0, 1 
    slt   $t1, $s0, $t2 #test for numbers less than 1 (including 0)
    bne   $t1, $0, outofrange #branches if $t1 is 1
     
    move  $v0, $s0 #move $s0 to $v0 to return

    lw    $ra, ($sp)
    addi  $sp, $sp, 4

    jr    $ra

outofrange:
    la    $a0, errorRange #loads msg at errorRange into $a0 to print
    jal   printf #prints error msg
    jal   exit #stops program


collatz:

    .global collatz

    addiu $sp, $sp, -20
    sw    $ra, ($sp)

    jal   collatz_atoi #calls function to convert argument to int

    move  $s0, $v0 #sets $s0 equal to $v0
    move  $a0, $v0 #sets $a0 equal to $v0
    
    jal collatz_print #prints first value

loop:
    addi  $t1, $0, 1 
    addi  $t2, $0, 2 #useful numbers for arithmatic functions
    addi  $t3, $0, 3

    beq   $s0, $t1, done #tests if $s0 is equal to 1
    move  $t0, $s0 #copies $s0 into $t0 for testing and arithmatic functions
    div   $t0, $t2 #divide by 2 to check if even
    mfhi  $t4 #checks if remainder is 0
    beq   $t4, $0, even #if remainder is 0 branch to even
    j odd
even:
    move  $t0, $s0
    div   $t0, $t2 #divide by 2
    mflo  $s0
    move  $a0, $s0 #copy into $a0 to print
    jal collatz_print
    j loop #jump to start of loop
odd:
    add   $t0, $s0, $0
    mult  $t0, $t3 #multiply by 3
    mfhi  $t5
    mflo  $t6
    or    $t0, $t5, $t6
    addi  $s0, $t0, 1 #add 1
    move  $a0, $s0 
    jal collatz_print #print value
    j loop #jump to loop
done:
    lw    $ra, ($sp)
    addiu $sp, $sp, 20

    jr $ra


