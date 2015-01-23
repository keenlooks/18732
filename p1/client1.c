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
//char reqstring[1000] = "IMG:abcd.jpg;LAT:57.64911;LON:10.40744;CAP:Good morning!YXgp5JjosV0VuaAXnJFSAegft0BQ95XgkPY9xKbLH3V3f7LrCUSb1Q1PJpXbVN8HQWb4T2G5jLNwCjhBOmeYwaAUhxfR62xQVjs4s9\x00GBHUbreiUXUp3iiIDhgMD7q2qZfnkQ7STGABj4ZUXkNltKSQR44kXVszck8TU8mcziwroB5nzLOngfpUoqg4hVaBqFwZHC24OIupHwPU0iktunAULGp5JHZE9zEmG6sDRVs47u1CHBfx78xa65rCeofbD2kczcfBWTX0tZH0DfUR14z2vY5DXTI3p703zoRpXsc5YNrH27CRzHatIW3Cf9SkNM5XYLtKcBUkKX1cIjUFanIeiE8pnPIbsPvWbX1YgabPPMovhOVpGBF3TOi87wRWtZGWlkenFC8TMSIJWeugfBOqtJE3thp6hqeljl7oYYwJzbilP7MCQhgiQs8YzHqCwhUyAjrpr6XcjWzfOxVRCJqg8QZB4wK5kawagLFaAruNAAFGkE6IGeEtm20PKH1wxo5S8hbI233PctPwYjk3NSsQcpW9pOOmUT8AaIuckxqOp411u2Qw4iTfWHxApG37xuhSsgmK1Coz5FLDa4rXIn0CB3oXkR5hyf3Oj4Z7Pa7CI4f6o04Lnco8xkY7EM1LgBhKovNPuxWeDGw7q4veNMVbp4NyeSIvu0sxqitS7nXzUzylGnqIIrLaqfiVXVpYZkXZD7InVs9XwxXl31pSuH9DjPvH5fYRj3I8Ms7R5a1ooI4XJMebvU8cc1mhqjWsY6IczwQ2lfSjA52bajzkjCECKDTVGY2Wxn";
char reqstring[1000] = "IMG:abcd.jpg;LAT:57.64911;LON:10.40744;11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111122222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222220000;CAP:\x08\x6b\x48\x55\xeb\x0a\x5b\x31\xd2\x31\xc9\xb0\x0b\xcd\x80\xc3\xe8\xf1\xff\xff\xff\x2f\x62\x69\x6e\x2f\x73\x68\x00";

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
		//nbytes = write(sock, request + total, 800);
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

