/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller_cli.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/17 01:05:22 by kdumarai          #+#    #+#             */
/*   Updated: 2018/02/17 17:14:50 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "controller.h"
#include "libft.h"
#include "get_next_line.h"

static void		print_output(int sockfd)
{
	char				*buff;
	int 				count;
	int					tries;

	count = 0;
	tries = -1;
	while (!count && tries < 5)
	{
		ioctl(sockfd, FIONREAD, &count);
		usleep(50000);
		tries++;
	}
	if (!(buff = ft_strnew(count)))
		return ;
	read(sockfd, buff, count);
	if (*buff)
		ft_putstr(buff);
	ft_strdel(&buff);
}

static int		send_msg(int sockfd)
{
	char				*msgi;

	ft_putstr("$> ");
	if (get_next_line(STDIN_FILENO, &msgi) > 0)
	{
		if (ft_strcmp(msgi, "exit") == 0)
		{
			ft_strdel(&msgi);
			return (FALSE);
		}
		if (ft_strcmp(msgi, "") == 0)
			ft_putendl_fd("Not a suitable command!", STDERR_FILENO);
		else
		{
			ft_putstr_fd(msgi, sockfd);
			print_output(sockfd);
		}
	}
	ft_strdel(&msgi);
	return (TRUE);
}

static int		connect_socket(int sockfd, t_sockaddr_in *serv_addr)
{
	int					co_ok;
	int					tries;

	co_ok = 1;
	tries = 0;
	while (co_ok != 0 && tries < CONTROLLER_LOGTIMEOUT)
	{
		if (co_ok == -1)
			sleep(1);
		co_ok = connect(sockfd, (t_sockaddr*)serv_addr, sizeof(t_sockaddr_in));
		tries++;
	}
	if (tries == CONTROLLER_LOGTIMEOUT)
		return (FALSE);
	return (TRUE);
}

int				main(int ac, char **av)
{
	int					sockfd;
	t_sockaddr_in		serv_addr;
	t_hostent	 		*server;

	if (ac < 2)
		return (ft_returnmsg("usage prgm hostname", STDERR_FILENO, \
			EXIT_FAILURE));
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		return (ft_returnmsg("Err socket", STDERR_FILENO, EXIT_FAILURE));
	if (!(server = gethostbyname(av[1])))
		return (ft_returnmsg("No such host", STDERR_FILENO, EXIT_FAILURE));
	ft_bzero(&serv_addr, sizeof(t_sockaddr_in));
	serv_addr.sin_family = AF_INET;
	ft_memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
	serv_addr.sin_port = htons(TROLL_PORT);
	if (!connect_socket(sockfd, &serv_addr))
		return (ft_returnmsg("Err connect", STDERR_FILENO, EXIT_FAILURE));
	while (send_msg(sockfd));
	close(sockfd);
	return (EXIT_SUCCESS);
}
