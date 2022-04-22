.option pic0

    .rdata
    .align 2

outputFormat:

    .asciz "%s"

    .align 2
    .text

.data

size:      .word  16
                        .align 4

buffer:    .space 64

.data

    fileName:.asciz "/usr/share/dict/words"
    .text

.data

    fileMode:.asciz "r"
    .text

.global main

main:
    
    addiu $sp, $sp, -4
    sw    $ra, ($sp)

    la    $a0, fileName
    la    $a1, fileMode

    jal   fopen

    move  $s0, $v0

Wordloop:


    la    $a0, buffer
    la    $a1, size
    move  $a2, $s0

    jal   fgets

    add   $s1, $v0, $0
    move  $s2, $s1
    beq   $s1, 0, done
 
    lb    $a1, 0($s1)
    slt   $t0, $a1, 97
    slt   $t1, $a1, 123
    bne   $t0, 0, noOp
    beq   $t1, 0, noOp
length:

    move  $a0, $s1
    jal   strlen
    move  $t1, $v0
palindrome:

    addi  $t1, $t1, -2 #does not count \n character in length
    li    $t0, 0
    slt   $t2, $t1, 2
    bne   $t2, 0, noOp
loop:

    add   $t3, $s2, $t1
    lbu   $t3, 0($t3)
    add   $t4, $s2, $t0
    lbu   $t4, 0($t4)
    slt   $t5, $t3, 97
    slt   $t6, $t3, 123
    bne   $t5, 0, noOp
    beq   $t6, 0, noOp
    slt   $t5, $t4, 97
    slt   $t6, $t4, 123
    bne   $t5, 0, noOp
    beq   $t6, 0, noOp
    bne   $t3, $t4, noOp
    beq   $t1, $t0, print 
    addi  $t1, $t1, -1
    beq   $t1, $t0, print
    addi  $t0, $t0, 1

    j     loop
    
    
print:   
    move  $a1, $s2
    la    $a0, outputFormat
    jal   printf

noOp:    
    j     Wordloop

done:

    lw    $ra, ($sp)
    addiu $sp, $sp, 4

    jal exit
