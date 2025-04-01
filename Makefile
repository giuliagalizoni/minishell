# rule for testing
test:
	cc -g -Wall -Wextra -Werror path_utils.c executer.c parser.c main.c -Llibft -lreadline libft/libft.a
