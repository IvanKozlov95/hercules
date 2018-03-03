/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikozlov <ikozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/01 20:45:55 by ikozlov           #+#    #+#             */
/*   Updated: 2018/03/02 21:47:59 by ikozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	die(char *msg, FILE *fd)
{
	if (fd)
		fputs(msg, fd);
	puts(msg);
	exit(1);
}

void	ft_log(char *msg, FILE *fd)
{
	if (fd)
		fputs(msg, fd);
	puts(msg);
}

void	parse_args(int ac, const char *av[], int *daemon, int *port)
{
	char	*msg;
	int		res;
	
	if (ac != 2 && ac != 3)
	{
		asprintf(&msg, "usage %s: [port] [-D]\n", av[0]);
		die(msg, NULL);
	}
	if ((*port = atoi(av[1])) == 0)
		die("Port is incorrect\n", NULL);
	*daemon = (ac == 3) && (strcmp(av[2], "-D") == 0) ? 1 : 0;
}

int		start_server(int port, FILE *out_fd)
{
	struct sockaddr_in	addr;
	socklen_t			len;
	int					n_sock;
	int					c_sock;
	int					nread;
	char				buf[BUFF_SIZE + 1];

	c_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (c_sock == -1)
		die("Issue with socket()\n", out_fd);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);
	if (bind(c_sock, (struct sockaddr *)&addr, sizeof(addr)) != 0)
		die("bind() error\n", out_fd);
	while (1)
	{
		if (listen(c_sock, 10) < 0)
			die("listen() error\n", out_fd);
		if ((n_sock = accept(c_sock, (struct sockaddr *)&addr, &len)) < 0)
			die("accept() error\n", out_fd);
		while ((nread = recv(n_sock, buf, BUFF_SIZE, 0)) > 0)
		{
			buf[nread] = '\0';
			if (strcmp(buf, KEY_WORD) == 0)
				write(n_sock, PONG_PONG, 9);
			else
				write(n_sock, "\0", 1);
		}
		close(n_sock);
	}
	close(c_sock);
}

int		main(int ac, const char *av[])
{
	int					port;
	int					daemon;
	int					pid;
	char				*msg;
	FILE				*fd;
	
	fd = NULL;
	parse_args(ac, av, &daemon, &port);
	if (daemon)
	{
		pid = fork();
		if (pid < 0)
			die("fork() fail\n", NULL);
		else if (pid > 0)
		{
			printf("child's pid = %d\n", pid);
			exit(0);
		}
		else
		{
			if (setsid() < 0)
			 	exit(1);
			chdir("/");
			close(STDIN_FILENO);
			close(STDOUT_FILENO);
			close(STDERR_FILENO);
			fd = fopen(LOG_PATH, "w+");
		}
	}
	start_server(port, NULL);
	return (0);
}