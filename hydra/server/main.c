/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikozlov <ikozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/01 20:45:55 by ikozlov           #+#    #+#             */
/*   Updated: 2018/03/03 14:55:44 by ikozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

FILE	*fd;

void	die(char *msg)
{
	fd ? fputs(msg, fd) : puts(msg);
	exit(1);
}

void	ft_log(char *msg)
{
	if (fd)
	{
		fprintf(fd, "%s\n", msg);
		fflush(fd);
	}
	else
		printf("%s\n", msg);
}

void	parse_args(int ac, const char *av[], int *daemon, int *port)
{
	char	*msg;
	int		res;
	
	if (ac != 2 && ac != 3)
	{
		asprintf(&msg, "usage %s: [port] [-D]", av[0]);
		die(msg);
	}
	if ((*port = atoi(av[1])) == 0)
		die("Port is incorrect");
	*daemon = (ac == 3) && (strcmp(av[2], "-D") == 0) ? 1 : 0;
}

void	*client_routine(void *data)
{
	int		client;
	FILE	*out_fd;
	int		nread;
	char	buf[BUFF_SIZE + 1];
	char	*msg;

	client = *(int *)data;
	while ((nread = recv(client , buf, BUFF_SIZE, 0)) > 0)
	{
		buf[nread] = '\0';
		asprintf(&msg, "Message from client #%d: %s", client, buf);
		ft_log(msg);
		if (strcmp(buf, KEY_WORD) == 0)
			write(client, PONG_PONG, 9);
		else
			write(client, "\0", 1);
	}
	ft_log("[INFO]: client has disconnected");
	close(client);
	free(data);
	return (0);
}

void	start_server(int port, FILE *out_fd)
{
	struct sockaddr_in	addr;
	socklen_t			len;
	int					client;
	int					con_sock;
	void				*data;
	pthread_t			thread;

	con_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (con_sock == -1)
		die("Issue with socket()");
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);
	if (bind(con_sock , (struct sockaddr *)&addr, sizeof(addr)) != 0)
		die("bind() error");
	if (listen(con_sock , 10) < 0)
		die("listen() error");
	while ((client = accept(con_sock , (struct sockaddr *)&addr, &len)))
	{
		ft_log("[INFO]: client has connected");
		data = malloc(1);
		data = (void *)&client;
		if (pthread_create(&thread, NULL, client_routine, data) < 0)
			die("thread error()");
	}
	close(con_sock);
}

int		main(int ac, const char *av[])
{
	int					port;
	int					daemon;
	int					pid;
	
	fd = NULL;
	parse_args(ac, av, &daemon, &port);
	if (daemon)
	{
		pid = fork();
		if (pid < 0)
			die("fork() fail");
		else if (pid > 0)
		{
			printf("child's pid = %d\n", pid);
			exit(0);
		}
		else
		{
			umask(0);
			if (setsid() < 0)
			 	exit(1);
			chdir("/");
			if ((fd = fopen(LOG_PATH, "w+")))
				printf("See logs in %s\n", LOG_PATH);
			else
				printf("Error creating log file at %s\n", LOG_PATH);
			close(STDIN_FILENO);
			close(STDOUT_FILENO);
			close(STDERR_FILENO);
		}
	}
	start_server(port, fd);
	return (0);
}