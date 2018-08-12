/* ******************************--------************************************ */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller_cli.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: someone <someone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/mm/dd hh:mm:ss by someone           #+#    #+#             */
/*   Updated: 2018/08/12 14:49:56 by kdumarai         ###   ########.fr       */
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

static void		print_output(int sockfd)
{
	char				*buff;
	int					count;
	int					tries;

	count = 0;
	tries = -1;
	while (!count && tries < 5)
	{
		(void)ioctl(sockfd, FIONREAD, &count);
		usleep(50000);
		tries++;
	}
	if (!(buff = ft_strnew(count)))
		return ;
	(void)read(sockfd, buff, count);
	if (*buff)
		ft_putstr(buff);
	ft_strdel(&buff);
}

static void		show_retry_msg(int tries)
{
	if (tries > 1)
		ft_putstr("\033[0A\033[K");
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

	tries = -1;
	while (++tries < CONTROLLER_LOGTIMEOUT)
	{
		if (tries > 0)
			show_retry_msg(tries);
		if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			return (ft_returnmsg("Err socket", STDERR_FILENO, FALSE));
		if (!(server = gethostbyname(hostname)))
			return (ft_returnmsg("No such host", STDERR_FILENO, FALSE));
		ft_bzero(&serv_addr, sizeof(t_sockaddr_in));
		serv_addr.sin_family = AF_INET;
		(void)ft_memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, \
			server->h_length);
		serv_addr.sin_port = htons(TROLL_PORT);
		if (!connect(sockfd, (t_sockaddr*)&serv_addr, sizeof(t_sockaddr_in)))
			return (sockfd);
	}
	if (tries == CONTROLLER_LOGTIMEOUT)
		return (FALSE);
	return (sockfd);
}

static void		controller_usage(const char *app_name)
{
	ft_putstr_fd("usage: ", STDERR_FILENO);
	ft_putstr_fd(app_name, STDERR_FILENO);
	ft_putendl_fd(" hostname", STDERR_FILENO);
	exit(EXIT_FAILURE);
}

int				main(int ac, char **av)
{
	int					sockfd;
	t_rl_opts			opts;
	t_uint8				interactive;
	t_dlist				*hist;

	if (ac < 2)
		controller_usage(av[0]);
	if (!ft_strequ(getenv("TERM"), "xterm-256color"))
	{
		return (ft_returnmsg("You must use xterm-256color!",
			STDERR_FILENO, EXIT_FAILURE));
	}
	if (!(sockfd = connect_socket(av[1])))
		return (ft_returnmsg("Err connect", STDERR_FILENO, EXIT_FAILURE));
	hist = NULL;
	interactive = isatty(STDIN_FILENO);
	ft_bzero(&opts, sizeof(t_rl_opts));
	opts.tbell = YES;
	while (send_msg(sockfd, &opts, &hist, interactive))
		print_output(sockfd);
	ft_dlstdel(&hist, &ftrl_histdelf);
	(void)close(sockfd);
	return (EXIT_SUCCESS);
}
