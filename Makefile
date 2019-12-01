# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pnita <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/04/07 17:43:11 by pnita             #+#    #+#              #
#    Updated: 2019/04/07 18:30:46 by pnita            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = lem-in
FLAGS = -I./include/ #-Wall -Werror -Wextra
HEADER = include
DIR_S = srcs
DIR_O = objs
SOURCES = lem-in.c reading_and_check_valid.c prepare.c links.c way.c bfs.c koords.c
SRCS = $(addprefix $(DIR_S)/,$(SOURCES))
OBJS = $(addprefix $(DIR_O)/,$(SOURCES:.c=.o))
all: $(NAME)
$(NAME): $(OBJS)
	@make -C ft_printf
	@gcc -o $(NAME) $(FLAGS) $(OBJS) ft_printf/libftprintf.a
$(DIR_O)/%.o: $(DIR_S)/%.c
	@mkdir -p objs
	@gcc $(FLAGS) -I $(HEADER) -o $@ -c $<
clean:
	@make clean -C ft_printf
	@rm -rf $(DIR_O)
fclean: clean
	@make fclean -C ft_printf
	@rm -f $(NAME)
re: fclean all