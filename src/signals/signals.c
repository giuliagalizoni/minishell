#include "../includes/minishell.h"

void	sigint_reset_prompt(int signal)
{
	(void)signal;
	g_signal_code = signal;
	ft_putchar_fd('\n', 2);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sigint_newline(int signal)
{
	g_signal_code = signal;
	ft_putchar_fd('\n', 2);
	rl_on_new_line();
}

void	sigquit_newline(int signal)
{
	char	*message;
	char	*errnum;

	errnum = ft_itoa(signal);
	if (!errnum)
		return ;
	message = ft_strjoin("Quit ", errnum);
	g_signal_code = signal;
	ft_putstr_fd(message, 2);
	ft_putchar_fd('\n', 2);
	free(message);
	free(errnum);
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
	struct sigaction	sa_sigint;
	struct sigaction	sa_sigquit;

	ft_bzero(&sa_sigint, sizeof(sa_sigint));
	ft_bzero(&sa_sigquit, sizeof(sa_sigquit));
	sa_sigint.sa_handler = &sigint_newline;
	sa_sigquit.sa_handler = &sigquit_newline;
	sa_sigint.sa_flags = SA_RESTART;
	sa_sigquit.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_sigint, NULL);
	sigaction(SIGQUIT, &sa_sigquit, NULL);
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
