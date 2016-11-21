#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define GET_LOG 100
#define SEND_LOG 101
#define FAIL_SEND 102
#define EXIT_CMD 900
#define EXIT_PROCESS 901
#define MAX 1024


void error_handling(char *message);



int main(int argc, char *argv[])
{
	int server_sock;
	int client_sock;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	int clnt_addr_size;
	char message[] = "Hello!";


	FILE *fp;
	fp = fopen("test_log", "r");

	char file[MAX]; // string from fopen
	char readmsg[MAX];
	char *tmp;
	char *split;
	int len;
	int n, i = 0;

	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	server_sock = socket(PF_INET, SOCK_STREAM, 0); /* 서버 소켓 생성 */
	if (server_sock == -1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if (bind(server_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) /* 소켓에 주소 할당 */
		error_handling("bind() error");

	if (listen(server_sock, 5) == -1)  /* 연결 요청 대기 상태로 진입 */
		error_handling("listen() error");

	for (;;) {

		clnt_addr_size = sizeof(clnt_addr);
		client_sock = accept(server_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size); /* 연결 요청 수락 */

		if (client_sock == -1)
			error_handling("accept() error");

		for (;;) {
			len = 0;
			tmp = readmsg;
			while ((n = read(client_sock, tmp, 1))>0) // input by client
			{
				if (*tmp == '\r') continue;
				if (*tmp == '\0') break;
				if (*tmp == '\n') break;

				if (len == MAX) break;

				tmp++;
				len++;
			}

			readmsg[len] = '\0';
			if (strcmp(readmsg, "GET") == 0)
			{
				fgets(file, sizeof(file), fp);
				split = strtok(file, "\n");
				n = strlen(split);
				printf("<%s>\n", split);
				write(client_sock, split, n);
				memset(readmsg, '\0', sizeof(readmsg));
			}
			else if (strcmp(readmsg, "EXIT") == 0)
			{
				write(client_sock, "EXIT PRC", 8);
				memset(readmsg, '\0', sizeof(readmsg));
			}
			else
			{
				close(client_sock);
				break;
			}
		}
		// write(client_sock, test, strlen(test)); /* 데이터 전송 */  
		// printf("Message in server : %s \n", test); 
		// close(client_sock); /* 연결 종료 */
	}
	fclose(fp);
	//return 0;


}



void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);

}