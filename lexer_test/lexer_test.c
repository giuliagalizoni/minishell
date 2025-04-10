#include "../includes/minishell.h"

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

    printf(MAGENTA "Input redirect: " RESET "%s\n", command->input_redirect ? command->input_redirect : "(none)");
    printf(MAGENTA "Heredoc delimiter: " RESET "%s\n", command->heredoc_delimiter ? command->heredoc_delimiter : "(none)");
    printf(MAGENTA "Is heredoc: " RESET "%d\n", command->is_heredoc);
    printf(MAGENTA "Append output: " RESET "%d\n", command->append_output);
    printf(MAGENTA "Output redirect: " RESET "%s\n", command->output_redirect ? command->output_redirect : "(none)");
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

// Main function to test lexer and parser
int main(int argc, char **argv, char **envp)
{
    if (argc < 2)
    {
        printf(RED "Usage: %s <command_line>\n" RESET, argv[0]);
        return 1;
    }

    char *line = argv[1];
    printf(BLUE "Input line: " RESET "%s\n\n", line);

    // Test lexer
    char **tokens = NULL;
    tokens = lexer(line, &tokens);
    if (!tokens)
    {
        printf(RED "Lexer failed.\n" RESET);
        return 1;
    }
    print_tokens(tokens);

    // Test parser
    t_command command;
    parser(line, &command, envp);
    print_command_list(&command);

    // Free memory
    free_arr((void **)tokens);
	clear_command_chain(&command);
    return 0;
}
