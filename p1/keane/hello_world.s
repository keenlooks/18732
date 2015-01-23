.section .text
.globl _start
_start:
    nop
    pushq $4; popq %rax
    pushq $1; popq %rdi	
    movq $0xAAAA0a21646c726f, %rcx
    pushq %rcx 
    movq $0x57202c6f6c6c6548, %rcx
    pushq %rcx
    xorq %rbx, %rbx
    xorq %rcx, %rcx
    movl $14, %ecx
    movw %bx, (%rsp, %rcx, 1)
    movq %rsp, %rsi
    pushq $14; popq %rdx
    int $0x80
    popq %rcx
    popq %rcx    
    pushq $1; popq %rax
    pushq $0; popq %rdi
    int $0x80
