# rule for testing
test:
	cc -g -Wall -Wextra -Werror   executer.c parser.c main.c -Llibft -lreadline libft/libft.a
