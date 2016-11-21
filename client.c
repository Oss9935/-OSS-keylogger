#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <malloc.h>

#define GRT_LOG 100
#define SEND_LOG 101
#define FAIL_SEND 102
#define EXIT_CMD 900
#define EXIT_PRC 901
#define MAX 32

void error_handling(char *message);

char input[MAX]; // string that will send to server

int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	char message[MAX]; // array that will store string from server 
	int str_len;

	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);  /* 서버 접속을 위한 클라이언트 소켓 생성 */
	if (sock == -1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) /* 서버로 연결 요청 */
		error_handling("connect() error!");

 	scanf("%s", input);
	input[strlen(input)] = '\0';
	write(sock, input , strlen(input)+1); // send input string to server


	str_len = read(sock, message, sizeof(message) - 1); /* 데이터 수신 */
	if (str_len == -1)
		error_handling("read() error!");

	message[str_len] = '\0';
	printf("Message from server : %s\n", message); // print msg from server

	close(sock); /* 연결 종료 */

	return 0;
}



void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
