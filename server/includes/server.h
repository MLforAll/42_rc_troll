/* ******************************--------************************************ */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: someone <someone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/mm/dd hh:mm:ss by someone           #+#    #+#             */
/*   Updated: 2018/mm/dd hh:mm:ss by someone          ###   ########.troll    */
/*                                                                            */
/* ******************************--------************************************ */

#ifndef SERVER_H
# define SERVER_H

# include "../../includes/troll.h"

# define KILL_LOG	1
# define REAPED_LOG	0

void	process_hdl(int sigc);

void	*send_output(void *param);
int		check_recv(int fd);
void	exec_command(char *msg, int outfd);

#endif
