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
    addi s0,x0,1    #result=1   
    add s1,x0,a0   #i=n
loop:
    beq s1,x0,label1    #if(i==0) goto label1
    mul s0,s0,s1     #result*=i
    addi s1,s1,-1   #i-=1
    jal x0,loop

label1:
    mv a0,s0    #a0=result
    jr ra
