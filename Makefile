# rule for testing
test:

	cc -g -Wall -Wextra -Werror command_utils.c path_utils.c array_utils.c executer.c lexer.c parser.c main.c -Llibft -lreadline libft/libft.a

