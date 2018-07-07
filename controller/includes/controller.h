/* ******************************--------************************************ */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: someone <someone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/mm/dd hh:mm:ss by someone           #+#    #+#             */
/*   Updated: 2018/mm/dd hh:mm:ss by someone          ###   ########.troll    */
/*                                                                            */
/* ******************************--------************************************ */

#ifndef CONTROLLER_H
# define CONTROLLER_H

# include "libft.h"
# include "libftreadline.h"
# include "../../includes/troll.h"

# define CONTROLLER_LOGTIMEOUT	60

typedef struct hostent	t_hostent;

int						send_msg(int sockfd, t_rl_opts *opts);

#endif
