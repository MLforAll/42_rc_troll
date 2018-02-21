/* ******************************--------************************************ */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: someone <someone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/mm/dd hh:mm:ss by someone           #+#    #+#             */
/*   Updated: 2018/mm/dd hh:mm:ss by someone          ###   ########.troll    */
/*                                                                            */
/* ******************************--------************************************ */

#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
#include "libft.h"

void	prepare_pipe_child(int fd[2], int fd2[2])
{
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	close(fd[1]);
	close(fd2[0]);
	dup2(fd[0], STDIN_FILENO);
	dup2(fd2[1], STDOUT_FILENO);
	dup2(fd2[1], STDERR_FILENO);
}

void	*send_output(void *arg)
{
	int			infd;
	int			outfd;
	char		buff[32];
	ssize_t		rb;

	infd = ((int*)arg)[0];
	outfd = ((int*)arg)[1];
	while ((rb = read(infd, buff, sizeof(buff))) > 0)
		write(outfd, buff, rb);
	return (NULL);
}

int		check_recv(int fd)
{
	char		buffer[32];

	if (recv(fd, buffer, sizeof(buffer), MSG_PEEK | MSG_DONTWAIT) == 0)
		return (TRUE);
	return (FALSE);
}
