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

void	sighandler_child(int signal)
{
	char	*errnum;
	char	*message;

	errnum = ft_itoa(signal);
	message = NULL;
	if (!errnum)
		return ;
	g_signal_code = signal;
	if (signal == SIGQUIT)
		message = ft_strjoin("Quit ", errnum);
	ft_putchar_fd('\n', 2);
	rl_on_new_line();
	if (message)
		free(message);
	free(errnum);
}

void	sigint_heredoc_handler(int signal)
{
	g_signal_code = signal;
}

