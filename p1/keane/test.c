#include <stdio.h>
main(){
	FILE * fd; 
	fd = fopen("foo.txt","w+");
	fprintf(fd, "You lose!");
}

