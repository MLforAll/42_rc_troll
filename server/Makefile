# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/12/20 21:41:19 by kdumarai          #+#    #+#              #
#    Updated: 2018/07/07 02:16:04 by kdumarai         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = troll_server
BIN = ../$(NAME)

CC_FLAGS = -Wall -Werror -Wextra
CC_LIB = -I includes -I ../libft/includes
LD_FLAGS = -L ../libft -lft

LIBFT = ../libft/libft.a

INCLUDES = includes/server.h \
		../includes/troll.h

SRCDIR = srcs
SRCFILES = server_cli.c server_cmd.c server_utils.c
SRCS = $(addprefix $(SRCDIR)/, $(SRCFILES))
NSRC = $(shell echo "$(SRCFILES)" | awk '{print NF}')
CSRC = 1

OBJDIR = objs
OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

PROJTEXT = \033[1;36m$(NAME): \033[0;39m

all: $(BIN)

$(LIBFT): force
	@ make -C $(dir $(LIBFT))

$(BIN): $(LIBFT) $(OBJS) $(INCLUDES)
	@ printf "\033[K$(PROJTEXT)Compiling\n"
	@ printf "$(PROJTEXT)Linking\n"
	@ gcc -o $(BIN) $(LD_FLAGS) $(OBJS)
	@ printf "$(PROJTEXT)\033[1;32mProgram built at $(NAME)\033[0;39m\n"

noflags:
	@ printf "\033[1;31m----------------------\n"
	@ printf "| CC_FLAGS DISABLED! |\n"
	@ printf "\r----------------------\033[0;39m\n"
	@ make all CC_FLAGS=""

fsanitize:
	@ printf "\033[1;31m------------------------------\n"
	@ printf "\r| ADDRESS SANITIZER ENABLED! |\n"
	@ printf "\r------------------------------\033[0;39m\n"
	@ make all LD_FLAGS="$(LD_FLAGS) -fsanitize=address"

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCLUDES)
	@ if [ ! -d $(dir $@) ]; then mkdir -p $(dir $@); fi
	@ printf "\033[K$(PROJTEXT)Compiling \033[1;33m$<"
	@ printf " %.0s" {1..$(shell expr 44 - $(shell printf "$<" | wc -m))}
	@ export LC_ALL=C; printf "\033[1;34m[%3.0f%%]\033[0;39m\r" "$(shell bc <<< "scale=1; $(CSRC) / $(NSRC) * 100")"
	@ $(eval CSRC = $(shell expr $(CSRC) + 1))
	@ gcc $(CC_FLAGS) $(CC_LIB) -c $< -o $@

clean: cleanp
	@ make clean -C $(dir $(LIBFT))

cleanp:
	@ rm -rf $(OBJDIR)
	@ printf "$(PROJTEXT)Removed objects\n"

fclean: clean
	@ rm -f $(BIN)
	@ printf "$(PROJTEXT)Removed $(NAME)\n"

fcleanp: cleanp
	@ rm -f $(BIN)
	@ printf "$(PROJTEXT)Removed $(NAME)\n"

rep: fcleanp all

re: fclean all

force:
	@ true

.PHONY: all lib noflags fsanitize clean fclean re cleanp fcleanp rep
