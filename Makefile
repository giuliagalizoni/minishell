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
	   parser.c\
	   lexer.c\
	   executer.c\
	   array_utils.c\
	   command_utils.c\
	   path_utils.c\
	   builtin_utils.c\
	   general_utils.c\
	   list_utils.c\
	   startup.c\
	   exit.c\
	   echo.c\
	   export.c\
	   cd.c\
	   cleanup.c\
	   env.c)

SRCFILES := $(MODULES) $(SRCDIR)/main.c
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
