NAME	:= minishell
CC := clang
CFLAGS	:= -Wall -Wextra -Werror
DEBUG ?= 1
ifeq ($(DEBUG), 1)
CFLAGS += -g
$(info Building with debug flags enabled: CFLAGS=$(CFLAGS))
endif
LDFLAGS := -lreadline -Llibft -l:libft.a

#TODO move srcs to src/
SRCDIR	:= .
OBJDIR	:= obj
SRCFILES := $(wildcard $(SRCDIR)/*.c)
OBJFILES := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o,$(SRCFILES))
LIBFT	:= libft/libft.a

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

# TODO this is a bit of a hack because we can only have one main so the tests
# have to be in separate folders so they are skipped by the wildcards, so in
# turn the source files have to be hardcoded here
# rule for testing
valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --suppressions=./suppressions/readline.supp ./minishell
lextest:
	cc $(CFLAGS) lexer_test/lexer_test.c command_utils.c path_utils.c array_utils.c executer.c parser.c lexer.c $(LDFLAGS) -o parsertest
exectest:
	cc $(CFLAGS) exec_test/exec_test.c command_utils.c path_utils.c array_utils.c executer.c parser.c lexer.c $(LDFLAGS) -o exec_test
