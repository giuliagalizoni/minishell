#include "../includes/minishell.h"

int	readline_interrupt_event_hook(void)
{
	if (g_signal_code == 2)
		rl_done = 1;
	return (0);
}

static void	sig_ignore(void)
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
	sa_sigint.sa_handler = &sighandler_child;
	sa_sigint.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_sigint, NULL);
	ft_bzero(&sa_sigquit, sizeof(sa_sigquit));
	sa_sigquit.sa_handler = &sighandler_child;
	sa_sigquit.sa_flags = SA_RESTART;
	sigaction(SIGQUIT, &sa_sigquit, NULL);
}

void	set_signals_heredoc(void)
{
	struct sigaction	sa_sigint;

	ft_bzero(&sa_sigint, sizeof(sa_sigint));
	sa_sigint.sa_handler = &sigint_heredoc_handler;
	sa_sigint.sa_flags = 0;
	sigaction(SIGINT, &sa_sigint, NULL);
	rl_catch_signals = 0;
	sig_ignore();
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
	rl_event_hook = 0;
}
