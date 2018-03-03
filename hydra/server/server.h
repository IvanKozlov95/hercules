/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikozlov <ikozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/01 09:59:00 by ikozlov           #+#    #+#             */
/*   Updated: 2018/03/02 21:39:33 by ikozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

# include <sys/socket.h>
# include <sys/types.h>
# include <netdb.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
#include <pthread.h>

# define PORT "1234"
# define PONG_PONG "pong pong"
# define KEY_WORD "ping"
# define LOG_PATH "/tmp/logs/server.txt"
# define MAX_CONNECTION (100)
# define BUFF_SIZE (1024)

#endif