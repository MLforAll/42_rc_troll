/* ******************************--------************************************ */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: someone <someone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/mm/dd hh:mm:ss by someone           #+#    #+#             */
/*   Updated: 2018/08/04 04:41:05 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ******************************--------************************************ */

#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "server.h"
#include <signal.h>
#include "libft.h"

pthread_mutex_t		g_mutex = PTHREAD_MUTEX_INITIALIZER;

static void			log_msg(pid_t pid, const char *act)
{
	ft_putstr("Program with pid: ");
	ft_putnbr(pid);
	ft_putstr(" got ");
	ft_putendl(act);
}

void				process_hdl(int sigc)
{
	int		status;
	pid_t	pid;

	if (sigc != SIGCHLD)
		return ;
	(void)pthread_mutex_lock(&g_mutex);
	if ((pid = waitpid(0, &status, WNOHANG | WUNTRACED)) > 0)
	{
		if (WIFSTOPPED(status))
		{
			if (KILL_LOG)
				log_msg(pid, "killed");
			(void)kill(pid, SIGKILL);
			(void)waitpid(0, NULL, WNOHANG);
		}
		else if (REAPED_LOG)
			log_msg(pid, "reaped");
	}
	(void)pthread_mutex_unlock(&g_mutex);
}

static void			launch_get_output(int infd, int outfd)
{
	int				args[2];
	pthread_t		thread;

	args[0] = infd;
	args[1] = outfd;
	pthread_create(&thread, NULL, &send_output, (void*)&args);
	pthread_detach(thread);
}

void				exec_command(char *msg, int outfd)
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
