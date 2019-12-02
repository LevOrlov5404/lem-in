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
FLAGS = -I./include/ -Wall -Werror -Wextra
HEADER = include
DIR_S = srcs
DIR_O = objs
SOURCES = lem_in.c reading1.c reading2.c prepare1.c links.c way.c bfs.c bfs_after.c koords.c delete.c prepare2.c prepare3.c output.c
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