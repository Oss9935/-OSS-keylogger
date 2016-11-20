#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUF_SIZE 256

int main()
{
	struct stat file_info;
	FILE *fp;
	char *str;
	int fileSize;
	char buf[BUF_SIZE];

	// system("cat /var/log/kern.log > /tmp/log");

	system("cat /var/log/kern.log");

	stat( "/var/log/kern.log", &file_info);

	fileSize = file_info.st_size;
	

	if ( fp = fopen( "/var/log/kern.log", "r")){
		printf("open success\n");

//		str = malloc(fileSize+1024);
//		memset(str, '\0', fileSize+1024);
//		printf("malloc success\n");

		memset(buf, '\0', BUF_SIZE);
		while(fgets(buf, BUF_SIZE, fp))
			puts(buf);
			//Todo : puts -> send

	printf("\n\n\nfile size is = %d\n", fileSize);


		fclose(fp);
//		free(str);
	}


	return 0;
}
