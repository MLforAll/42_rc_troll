/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/17 03:53:46 by kdumarai          #+#    #+#             */
/*   Updated: 2018/02/17 22:27:24 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/socket.h>
#include "libft.h"

void	prepare_pipe_child(int fd[2], int fd2[2])
{
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(fd[1]);
	close(fd2[0]);
	dup2(fd[0], STDIN_FILENO);
	dup2(fd2[1], STDOUT_FILENO);
}

void	send_output(int infd, int outfd)
{
	char		buff[32];
	ssize_t		rb;

	while ((rb = read(infd, buff, sizeof(buff))) > 0)
		write(outfd, buff, rb);
}

int		check_recv(int fd)
{
	char		buffer[32];

	if (recv(fd, buffer, sizeof(buffer), MSG_PEEK | MSG_DONTWAIT) == 0)
		return (TRUE);
	return (FALSE);
}
