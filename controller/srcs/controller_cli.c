/* ******************************--------************************************ */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller_cli.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: someone <someone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/mm/dd hh:mm:ss by someone           #+#    #+#             */
/*   Updated: 2018/mm/dd hh:mm:ss by someone          ###   ########.troll    */
/*                                                                            */
/* ******************************--------************************************ */

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
	int					count;
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

static void		show_retry_msg(int tries)
{
	ft_putstr("Tying again (");
	ft_putnbr(tries);
	ft_putchar('/');
	ft_putnbr(CONTROLLER_LOGTIMEOUT);
	ft_putendl(")");
	sleep(1);
}

static int		connect_socket(char *hostname)
{
	int					sockfd;
	t_sockaddr_in		serv_addr;
	t_hostent			*server;
	int					tries;

	tries = 0;
	while (tries < CONTROLLER_LOGTIMEOUT)
	{
		if (tries > 0)
			show_retry_msg(tries);
		if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			return (ft_returnmsg("Err socket", STDERR_FILENO, EXIT_FAILURE));
		if (!(server = gethostbyname(hostname)))
			return (ft_returnmsg("No such host", STDERR_FILENO, EXIT_FAILURE));
		ft_bzero(&serv_addr, sizeof(t_sockaddr_in));
		serv_addr.sin_family = AF_INET;
		ft_memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
		serv_addr.sin_port = htons(TROLL_PORT);
		if (connect(sockfd, (t_sockaddr*)&serv_addr, sizeof(t_sockaddr_in)) == 0)
			return (sockfd);
		tries++;
	}
	if (tries == CONTROLLER_LOGTIMEOUT)
		return (FALSE);
	return (sockfd);
}

int				main(int ac, char **av)
{
	int					sockfd;

	if (ac < 2)
	{
		ft_putstr_fd("usage: ", STDERR_FILENO);
		ft_putstr_fd(av[0], STDERR_FILENO);
		ft_putendl_fd(" hostname", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (!(sockfd = connect_socket(av[1])))
		return (ft_returnmsg("Err connect", STDERR_FILENO, EXIT_FAILURE));
	while (send_msg(sockfd))
		print_output(sockfd);
	close(sockfd);
	return (EXIT_SUCCESS);
}