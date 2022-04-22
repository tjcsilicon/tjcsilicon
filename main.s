.option pic0
    .rdata
    .align 2

#outputFormat:
 #   .asciz "%s\n"
    
  #  .align 2
   # .text
 
.global main

.data
    errorArg: .asciiz "ERROR INVALID ARGUMENTS\n"
    .text
main:

    addiu $sp, $sp, -4
    sw    $ra, ($sp)
    
    lw    $a0, 4($a1)  
    lw    $t0, 8($a1)

    beq   $a0, $0, noArg #tests if there are no arguments
    bne   $t0, $0, noArg #tests if there are 2 or more arguments

    jal collatz #call collatz function

    lw    $ra, ($sp)
    addiu $sp, $sp, 4

    jal  exit

noArg:
    la    $a0, errorArg
    jal   printf
    jal   exit 
