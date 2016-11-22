#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <malloc.h>

#define GET_LOG 100
#define SEND_LOG 101
#define FAIL_SEND 102
#define EXIT_CMD 900
#define EXIT_PROCESS 901
#define MAX 1024

void error_handling(char *message);


char cmd[MAX];

int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	char message[MAX];
	int str_len;
	int i = 0;



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

	for (;;) // cmd loop
	{
		printf("\nInput the CMD>>");
		scanf("%s", cmd); // input the CMD
		cmd[strlen(cmd)] = '\0';
		write(sock, cmd, strlen(cmd) + 1);

		if (strcmp(cmd, "GET") == 0)
		{
			memset(cmd, '\0', sizeof(cmd));
			printf("Message from server>>\n");

			for (;;) {
				memset(message, '\0', sizeof(message)); // memory set	
				str_len = read(sock, message, sizeof(message) - 1); /* 데이터 수신 */

				if (str_len == -1)
					error_handling("read() error!"); // manage error

				message[str_len] = '\0';
				printf("<%s>", message);
				if (strcmp(message, "OK") == 0)
				{
					write(sock, "SEND", 5);
					memset(message, '\0', sizeof(message));
					read(sock, message, sizeof(message) - 1);
					if (strcmp(message, "EOF") == 0)
						break;
					printf("%s", message);
				}
			}
		}
		else if (strcmp(cmd, "END") == 0)
			break;

	}
	close(sock); /* 연결 종료 */

	return 0;
}



void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}