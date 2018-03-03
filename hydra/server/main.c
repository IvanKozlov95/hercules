/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikozlov <ikozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/01 20:45:55 by ikozlov           #+#    #+#             */
/*   Updated: 2018/03/02 22:01:01 by ikozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	die(char *msg, FILE *fd)
{
	fd ? fputs(msg, fd) : puts(msg);
	exit(1);
}

void	ft_log(char *msg, FILE *fd)
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
		die(msg, NULL);
	}
	if ((*port = atoi(av[1])) == 0)
		die("Port is incorrect", NULL);
	*daemon = (ac == 3) && (strcmp(av[2], "-D") == 0) ? 1 : 0;
}

void	start_server(int port, FILE *out_fd)
{
	struct sockaddr_in	addr;
	socklen_t			len;
	int					client;
	int					con_sock;
	int					nread;
	char				buf[BUFF_SIZE + 1];
	char				*msg;

	con_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (con_sock == -1)
		die("Issue with socket()", out_fd);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);
	if (bind(con_sock , (struct sockaddr *)&addr, sizeof(addr)) != 0)
		die("bind() error", out_fd);
	if (listen(con_sock , 10) < 0)
		die("listen() error", out_fd);
	{
		// ft_log("accept() error", out_fd);
		while ((nread = recv(client , buf, BUFF_SIZE, 0)) > 0)
		{
			buf[nread] = '\0';
			asprintf(&msg, "Message from client #%d: %s", client, buf);
			ft_log(msg, out_fd);
			if (strcmp(buf, KEY_WORD) == 0)
				write(client, PONG_PONG, 9);
			else
				write(client, "\0", 1);
		}
		close(client);
		ft_log("[INFO]: client has disconnected", out_fd);
	}
	close(con_sock);
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
			die("fork() fail", NULL);
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
			printf("See logs in %s\n", LOG_PATH);
		}
	}
	start_server(port, NULL);
	return (0);
}