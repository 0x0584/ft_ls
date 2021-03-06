#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/12/08 22:27:47 by archid-           #+#    #+#              #
#    Updated: 2020/02/06 20:50:56 by archid-          ###   ########.fr        #
#                                                                              #
#******************************************************************************#

DEBUG		?= 0
NAME		= ft_ls

FTDIR		= libft
LDFT		= -L$(FTDIR) -lft

SRCSDIR		= src
DEPSDIR		= include
OBJSDIR		= .obj

SRCS		:= $(shell find $(SRCSDIR) -name '*.c' -type f)
OBJS		:= $(patsubst $(SRCSDIR)/%.c, $(OBJSDIR)/%.o, $(SRCS))

CC			= gcc
CFLAGS		= -Wall -Wextra
DEPS		= -I$(FTDIR) -Iinclude

ifeq ($(DEBUG),1)
	CFLAGS += -g
else
	CFLAGS += -Werror
endif

all: $(NAME)

$(NAME): init $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(DEPS) $(LDFT)

$(OBJSDIR)/%.o: $(SRCSDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ $(DEPS)

init:
	@make -C $(FTDIR)

clean:
	@make clean -C $(FTDIR)
	@rm -rf $(OBJS)

fclean:
	@make fclean -C $(FTDIR)
	@rm -rf $(OBJSDIR) $(NAME)

re: fclean all

.PHONY: all init clean fclean re
