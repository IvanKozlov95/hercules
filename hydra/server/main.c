/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikozlov <ikozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/01 20:45:55 by ikozlov           #+#    #+#             */
/*   Updated: 2018/03/02 21:33:14 by ikozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	info(char *msg)
{
	puts(msg);
}

void	die(char *msg)
{
	puts(msg);
	exit(1);
}

void	parse_args(int ac, const char *av[], int *daemon, int *port)
{
	char	*msg;
	int		res;
	
	if (ac != 2 && ac != 3)
	{
		asprintf(&msg, "usage %s: [port] [-D]\n", av[0]);
		die(msg);
	}
	if ((*port = atoi(av[1])) < 0)
		die("Port is incorrect\n");
	*daemon = (ac == 3) && (strcmp(av[2], "-D") == 0) ? 1 : 0;
}

int		start_server(int port, int out_fd)
{
	struct sockaddr_in	addr;
	socklen_t			len;
	int					n_sock;
	int					c_sock;
	int					nread;
	char				buf[BUFF_SIZE + 1];

	c_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (c_sock == -1)
		die("Issue with socket()\n");
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);
	if (bind(c_sock, (struct sockaddr *)&addr, sizeof(addr)) != 0)
		die("bind() error\n");
	while (1)
	{
		if (listen(c_sock, 10) < 0)
			die("listen() error\n");
		if ((n_sock = accept(c_sock, (struct sockaddr *)&addr, &len)) < 0)
			die("accept() error\n");
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
	
	parse_args(ac, av, &daemon, &port);
	if (daemon)
	{
		pid = fork();
		if (pid < 0)
			die("fork() fail\n");
		else if (pid > 0)
		{
			asprintf(&msg, "child's pid = %d\n", pid);
			info(msg);
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
			// todo: set up log
			start_server(port, 1);
		}
	}
	start_server(port, 1);
	return (0);
}