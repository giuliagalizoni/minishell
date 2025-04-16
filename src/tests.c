#include "includes/minishell.h"

// ANSI color codes
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"

// Function to print the array of tokens
void print_tokens(char **tokens)
{
    printf(CYAN "Tokens:\n" RESET);
    for (int i = 0; tokens[i]; i++)
        printf(YELLOW "  Token[%d]: " RESET "%s\n", i, tokens[i]);
    printf("\n");
}

// Function to print a single command and its attributes
void print_command(t_command *command)
{
    printf(GREEN "Command index: " RESET "%d\n", command->index);
    printf(GREEN "Command name: " RESET "%s\n", command->name);

    if (command->arguments)
    {
        printf(GREEN "Arguments:\n" RESET);
        for (int i = 0; command->arguments[i]; i++)
            printf(YELLOW "  arg[%d]: " RESET "%s\n", i, command->arguments[i]);
    }
    else
        printf(GREEN "Arguments: " RESET "(none)\n");

    if (command->input_redirect)
    {
        printf(MAGENTA "Input redirects:\n" RESET);
        for (int i = 0; command->input_redirect[i]; i++)
            printf(YELLOW "  input[%d]: " RESET "%s\n", i, command->input_redirect[i]);
    }
    else
        printf(MAGENTA "Input redirects: " RESET "(none)\n");

		if (command->outfile)
		{
			printf(MAGENTA "Output redirects:\n" RESET);
			t_outfile *current_outfile = command->outfile;
			int i = 0;
			while (current_outfile)
			{
				printf(YELLOW "  outfile[%d]: " RESET "Filename: %s, Append: %d\n",
					   i, current_outfile->filename, current_outfile->is_append);
				current_outfile = current_outfile->next;
				i++;
			}
		}
    else
        printf(MAGENTA "Output redirects: " RESET "(none)\n");

    printf(MAGENTA "Heredoc delimiter: " RESET "%s\n", command->heredoc_delimiter ? command->heredoc_delimiter : "(none)");
    printf(MAGENTA "Is heredoc: " RESET "%d\n", command->is_heredoc);
    printf(MAGENTA "Is pipe: " RESET "%d\n", command->is_pipe);
    printf("\n");
}

// Function to print the entire command list
void print_command_list(t_command *command)
{
    t_command *current = command;
    printf(CYAN "Command List:\n" RESET);
    while (current)
    {
        print_command(current);
        current = current->pipe_next;
    }
}
