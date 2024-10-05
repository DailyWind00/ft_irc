# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mgallais <mgallais@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/11 11:27:37 by mgallais          #+#    #+#              #
#    Updated: 2024/08/06 14:34:07 by mgallais         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME    = ircserv
CC      = c++
RM      = rm -f
CFLAGS += -g -Wall -Wextra -Werror -std=c++98
INCLUDE = -I include -I include/classes
DEBUG   = 0

SRC		= src/main.cpp \
		  src/signals.cpp \
		  src/classes/Client.cpp \
		  src/classes/Server/Server.cpp \
		  src/classes/Server/Server_loop.cpp \
		  src/commands/parsing.cpp \
		  src/commands/parsing_utils.cpp \
		  src/classes/Channel.cpp \
		  src/commands/RPL_message.cpp #

OBJ		= $(SRC:.cpp=.o)

all:
	@if [ -f $(NAME) ]; then \
		printf "\033[1;92m\nircserv already compiled\n\033[0m"; \
		make -s redirect; \
	else \
		printf "\033[1;36m\nStarting ircserv compilation\n\e[0m"; \
		make -s redirect; \
	fi

redirect: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ)
	@printf "\033[1;92m >>> Ircserv compiled                                                     \n\033[0m"
	@if [ $(DEBUG) -eq 1 ]; then printf "\033[1;93m >>> Debug mode activated\n\033[0m"; fi
	@printf "\n\033[0m"

%.o: %.cpp
	@printf "\033[0;33mCompilation de %-33.33s\r\e[0m" $@
	@if [ $(DEBUG) -eq 1 ]; then $(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@ -D DEBUG=1; else $(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@; fi

clean:
	@$(RM) $(OBJ)
	@printf "\033[1;31m- Object files deleted\n\033[0m"

fclean: clean	
	@$(RM) $(NAME)
	@printf "\033[1;31m- Executable file deleted\n\033[0m"

re: fclean all

test:
	@make -s re
	@make -s clean
	@clear
	@echo "\033[1;92mDONE !"

debug: fclean
	@make -s DEBUG=1 all

.PHONY: all redirect clean fclean re test debug
