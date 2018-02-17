/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/17 04:11:02 by kdumarai          #+#    #+#             */
/*   Updated: 2018/02/17 04:33:38 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

# include "../../includes/troll.h"

void	prepare_pipe_child(int fd[2], int fd2[2]);
void	send_output(int infd, int outfd);
int		check_recv(int fd);

#endif
