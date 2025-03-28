# rule for testing
test:
	cc -Wall -Wextra -Werror   executer.c parser.c main.c -Llibft -lreadline libft/libft.a
