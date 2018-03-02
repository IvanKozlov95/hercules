/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikozlov <ikozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/01 20:45:55 by ikozlov           #+#    #+#             */
/*   Updated: 2018/03/01 22:00:30 by ikozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	die(char *msg)
{
	puts(msg);
	exit(1);
}

int		main(int ac, char *av[])
{
	struct sockaddr_in	addr;
	socklen_t			len;
	int					n_sock;
	int					c_sock;
	int					nread;
	char				buf[BUFF_SIZE + 1];

	c_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (c_sock == -1)
		die("Issue with socket()");
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(atoi(av[1]));
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
			if (strncmp(buf, KEY_WORD,4) == 0)
				write(n_sock, PONG_PONG, 9);
			else
				write(n_sock, buf, nread);
		}
		close(n_sock);
	}
	close(c_sock);
	return (0);
}