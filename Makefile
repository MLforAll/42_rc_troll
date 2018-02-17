# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/12/20 21:41:19 by kdumarai          #+#    #+#              #
#    Updated: 2018/02/17 04:14:49 by kdumarai         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

all:
	@ make -C controller
	@ make -C server

clean:
	@ make -C controller clean
	@ make -C server clean

fclean:
	@ make -C controller fclean
	@ make -C server fclean

re: fclean all

.PHONY: all clean fclean re
