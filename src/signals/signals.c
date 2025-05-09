#include "../includes/minishell.h"

void	sigint_reset_prompt(int signal)
{
	(void)signal;
	g_signal_code = signal;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	signal_newline(int signal)
{
	(void)signal;
	g_signal_code = signal;
	rl_on_new_line();
}

void	sig_ignore(void)
{
	struct sigaction	sa;

	ft_bzero(&sa, sizeof(sa));
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	sigaction(SIGQUIT, &sa, NULL);
}

void	set_signals_child(void)
{
	struct sigaction	sa;

	ft_bzero(&sa, sizeof(sa));
	sa.sa_handler = &signal_newline;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void	set_signals_parent(void)
{
	struct sigaction	sa_int;

	sig_ignore();
	ft_bzero(&sa_int, sizeof(sa_int));
	sa_int.sa_handler = &sigint_reset_prompt;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
}
