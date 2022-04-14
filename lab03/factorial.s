.globl factorial

.data
n: .word 8

.text
main:
    la t0, n
    lw a0, 0(t0)            #a0=n
    jal ra, factorial       #factorial(a0)

    addi a1, a0, 0          #a1=a0
    addi a0, x0, 1          #a0=1
    ecall # Print Result

    addi a1, x0, '\n'       #a1='\n'
    addi a0, x0, 11         #a0=11
    ecall # Print newline

    addi a0, x0, 10         #a0=10
    ecall # Exit


factorial:
    
    beq a0,x0,label1
label1:
    addi a0,x0,0
    jr ra

    beq a0,1,label2
label2:
    addi a0,x0,1
    jr ra

    addi t1,x0,0    #num1
    addi t2,x0,1    #num2
    addi t3,x0,0    #total
    addi t4,x0,2    #i
loop:
    addi t5,a0,1        #t5=a0+1
    bge t4,t5,label3    #if(t4>=a0+1) goto label3
    addi t3,t1,t2       #total=num1+num2
    mv t1,t2            #num1=num2
    mv t2,t3            #num2=total
    jal x0,loop

label3:
    mv a0,t3            #a0=total
    jr ra
