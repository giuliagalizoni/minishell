#include "includes/minishell.h"

void	sigquit_handler(int signal)
{
	//TODO need to 
	if (signal == SIGQUIT)
	{
		g_exit_status = 131;
		write(1, "\nbooo", 8);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	set_quit_action(void)
{
	struct sigaction sa_quit;

	ft_bzero(&sa_quit, sizeof(sa_quit));
	sa_quit.sa_handler = &sigquit_handler;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = SA_RESTART;
	// TODO wrap up in a in if sigaction == -1 for error check
	sigaction(SIGQUIT, &sa_quit, NULL);
}

struct sigaction	sigignore(int signal)
{
	struct sigaction	sa;
	struct sigaction	sa_old;

	ft_bzero(&sa, sizeof(sa));
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	sigaction(signal, &sa, &sa_old);

	return (sa_old);
}

void	sigint_handler(int signal)
{
	//TODO need to 
	printf("siging");
	if (signal == SIGINT)
	{
		g_exit_status = 130;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	set_signal_action(void)
{
	struct sigaction sa_int;

	ft_bzero(&sa_int, sizeof(sa_int));
	sa_int.sa_handler = &sigint_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	// TODO wrap up in a in if sigaction == -1 for error check
	sigaction(SIGINT, &sa_int, NULL);
	signal(SIGQUIT, SIG_IGN);
}
