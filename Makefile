# rule for testing
test:
	cc -Wall -Wextra -Werror   main.c -Llibft -lreadline libft/libft.a
exec:
	cc -Wall -Wextra -Werror   main_executer_tests.c -Llibft -lreadline libft/libft.a
