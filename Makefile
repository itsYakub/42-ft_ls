# ========

SHELL	= /usr/bin/zsh

# ========

CC		= cc
CFLAGS	= -Wall -Wextra -Werror -ggdb3
LFLAGS	= -lft
LDFLAGS	= -L$(LIBFT)

# ========

OBJS	= $(SRCS:.c=.o)
SRCS	= ./ft_ls.c			\
		  ./ft_ls-utils.c	\
		  ./ft_ls-getopt.c	\
		  ./ft_ls-print.c

# ========

LIBFT	= ./libft/

# ========

TARGET	= ./ft_ls

# ========


.PHONY : all

all : $(TARGET)


.PHONY : re

re : fclean all


.PHONY : fclean

fclean : clean
	make -C $(LIBFT) fclean
	rm -f $(TARGET)


.PHONY : clean

clean :
	make -C $(LIBFT) clean
	rm -f $(OBJS)


TEST_FILES	=
TEST_FLAGS	=

.PHONY : test

test : all
	@-diff <(/bin/ls $(TEST_FLAGS) $(TEST_FILES)) <($(TARGET) $(TEST_FLAGS) $(TEST_FILES))


.PHONY : tests

tests : all
	$(info Test: no flags)
	@make -C . test
	
	$(info Test: -a, --debug-syms)
	@make -C . test TEST_FLAGS=-l
	
	$(info Test: -g, --extern-only)
	@make -C . test TEST_FLAGS=-R
	
	$(info Test: -u, --undefined-only)
	@make -C . test TEST_FLAGS=-a
	
	$(info Test: -r, --reverse-sort)
	@make -C . test TEST_FLAGS=-r
	
	$(info Test: -p, --no-sort)
	@make -C . test TEST_FLAGS=-t

# ========

$(TARGET) : $(OBJS)
	make -C $(LIBFT) bonus
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LFLAGS)


$(OBJS) : %.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $<


# ========
