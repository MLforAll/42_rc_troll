/* ******************************--------************************************ */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: someone <someone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/mm/dd hh:mm:ss by someone           #+#    #+#             */
/*   Updated: 2018/mm/dd hh:mm:ss by someone          ###   ########.troll    */
/*                                                                            */
/* ******************************--------************************************ */

#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "server.h"
#include "libft.h"

static void		launch_get_output(int infd, int outfd)
{
	int				args[2];
	pthread_t		thread;

	args[0] = infd;
	args[1] = outfd;
	pthread_create(&thread, NULL, &send_output, (void*)&args);
	pthread_detach(thread);
}

void			exec_command(char *msg, int outfd)
{
	extern char		**environ;
	char			*bashav[4];
	pid_t			pid;
	int				fd[2];

	bashav[0] = "bash";
	bashav[1] = "-c";
	bashav[2] = msg;
	bashav[3] = NULL;
	ft_putstr("Issued: ");
	ft_putendl(msg);
	pipe(fd);
	if ((pid = fork()) == -1)
		return ;
	if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		dup2(fd[1], STDERR_FILENO);
		execve("/bin/bash", bashav, environ);
		exit(127);
	}
	close(fd[1]);
	launch_get_output(fd[0], outfd);
}
