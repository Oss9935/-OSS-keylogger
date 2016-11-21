#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>



void error_handling(char *message);

int main(int argc, char *argv[])

{
	int server_sock;
	int client_sock;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	int clnt_addr_size;
	char message[] = "Hello!\n";


	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	server_sock = socket(PF_INET, SOCK_STREAM, 0); /* ���� ���� ���� */
	if (server_sock == -1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if (bind(server_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) /* ���Ͽ� �ּ� �Ҵ� */
		error_handling("bind() error");

	if (listen(server_sock, 5) == -1)  /* ���� ��û ��� ���·� ���� */
		error_handling("listen() error");

	for (;;) {

		clnt_addr_size = sizeof(clnt_addr);
		client_sock = accept(server_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size); /* ���� ��û ���� */

		if (client_sock == -1)
			error_handling("accept() error");

		write(client_sock, message, sizeof(message)); /* ������ ���� */
		printf("Message in server : %s \n", message);
		close(client_sock); /* ���� ���� */
	}
	return 0;


}



void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);

}
