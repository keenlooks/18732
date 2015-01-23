//#include <unistd.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <fcntl.h>

int main()
{
    shellcode();
    //int fd = open("foo.txt", O_RDWR | O_CREAT, S_IRUSR | S_IRGRP | S_IROTH);
    //write(fd, "You lose!", 9);

    return 0;
}
