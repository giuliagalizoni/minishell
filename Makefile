NAME	:= minishell
CC := clang
CFLAGS	:= -Wall -Wextra -Werror
DEBUG ?= 1
ifeq ($(DEBUG), 1)
CFLAGS += -g
$(info Building with debug flags enabled: CFLAGS=$(CFLAGS))
endif
LIBFT	:= libft/libft.a
LDFLAGS := -lreadline -Llibft $(LIBFT)

#TODO move srcs to src/
SRCDIR	:= src
OBJDIR	:= obj
MODULES := $(addprefix $(SRCDIR)/,\
	   main.c\
	   startup/startup.c\
	   signals/signals.c\
	   parser/parser.c\
	   parser/lexer.c\
	   parser/check_syntax.c\
	   parser/expand_var.c\
	   parser/expand_inline.c\
	   parser/process_token.c\
	   executer/executer.c\
	   utils/array_utils.c\
	   utils/command_utils.c\
	   utils/path_utils.c\
	   utils/builtin_utils.c\
	   utils/general_utils.c\
	   utils/list_utils.c\
	   utils/cleanup_utils.c\
	   utils/expand_var_utils.c\
	   utils/export_utils.c\
	   utils/lexer_utils.c\
	   utils/error_utils.c\
	   builtins/exit.c\
	   builtins/echo.c\
	   builtins/export.c\
	   builtins/cd.c\
	   builtins/env.c\
	   builtins/pwd.c\
	   builtins/unset.c\
	   redirections/redirection.c\
	   redirections/heredoc.c\
	   utils/expand_inline_utils.c\
	   utils/parser_utils.c\
	   utils/ft_perror.c\
	   utils/process_token_utils.c\
	   utils/handle_double_quote.c)

SRCFILES := $(MODULES)
OBJFILES := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o,$(SRCFILES))
LEXERTESTS := $(MODULES) $(SRCDIR)/tests/tests_lexer.c

.PHONY: all
all: $(LIBFT) $(NAME)

$(NAME): $(OBJFILES)
	@echo "Linking $@..."
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $(NAME)
	@echo "The $@ has been built successfully"

$(OBJDIR)/%.o : $(SRCDIR)/%.c | $(OBJDIR)
	@echo "compiling $@"
	$(CC) $(CFLAGS) -Ilibft -c $< -o $@

$(OBJDIR):
	@echo "Creating directory $@..."
	@mkdir -p $@
	@mkdir -p $@/utils $@/startup $@/signals $@/builtins $@/redirections $@/parser $@/executer

$(LIBFT):
	make -C ./libft

.PHONY: clean
clean:
	make fclean -C libft

.PHONY: fclean
fclean: clean
	rm -rf $(OBJDIR)
	rm -f $(NAME)

.PHONY: re
re: fclean all

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --suppressions=./suppressions/readline.supp ./minishell
lextest:
	cc $(CFLAGS) $(LEXERTESTS) $(LDFLAGS) -o lexertest
