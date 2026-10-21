NAME = codexion
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

SRCS = arguments_validator.c coder.c dongles_logic.c heap_continue.c \
heap.c init_and_clean_data.c main.c monitor.c simulation.c time_and_logs.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(OBJS): codexion.h

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re