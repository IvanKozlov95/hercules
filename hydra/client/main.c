#include "client.h"

int main()
{
	int					fd;
	int					val;
	int					port;
	struct sockaddr_in	server_addr;
	char				buf[BUFF_SIZE + 1];

	fd = socket(AF_INET, SOCK_STREAM, 0);
	printf("Enter the port number\n");
	scanf("%d", &port);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr =  INADDR_ANY; 
	server_addr.sin_port = htons(port);
	if(connect(fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0)
	{
		printf("Connection to server failed!\n");
		exit(1);
	}
	printf("Connected to Server on port %d\n", port);
	while(1) 
	{
		
		printf("Message to server: ");
		bzero(buf, BUFF_SIZE);
		scanf("%s", buf);
		write(fd, buf, strlen(buf));
		bzero(buf, BUFF_SIZE);
		read(fd, buf, BUFF_SIZE);
		printf("Message from server: %s\n", buf);
		
	}
	close(fd);
	return (0);
}
