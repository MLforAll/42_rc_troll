# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/12/20 21:41:19 by kdumarai          #+#    #+#              #
#    Updated: 2018/07/07 02:01:40 by kdumarai         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

all:
	@ make -C controller
	@ make -C server

clean:
	@ make -C controller clean
	@ make -C server clean

cleanp:
	@ make -C controller cleanp
	@ make -C server cleanp

fclean:
	@ make -C controller fclean
	@ make -C server fclean

fcleanp:
	@ make -C controller fcleanp
	@ make -C server fcleanp

re: fclean all

rep: fcleanp all

.PHONY: all clean fclean re cleanp fcleanp rep
