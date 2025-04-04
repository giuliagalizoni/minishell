NAME	= minishell
CFLAGS	:= -Wall -Wextra -Werror
DEBUGFLAGS := -g
ARFLAGS	= rcs
#TODO move srcs to src/
SRCDIR	= src/
SRCFILES = command_utils.c path_utils.c array_utils.c executer.c parser.c main.c lexer.c
CFILES	= minishell
OBJDIR	= obj/
LIBFT	= libft/libft.a

SRC 	= $(addprefix $(SRCDIR), $(addsuffix .c, $(CFILES)))
BONUSSRC 	= $(addprefix $(SRCBONUSDIR), $(addsuffix .c, $(BONUSFILES)))


.PHONY: all
all: $(NAME)


$(NAME): $(LIBFT)
	cc $(CFLAGS) $(SRC) $(LIBFT) -o $(NAME)

$(LIBFT):
	make -C ./libft

.PHONY: clean
clean:
	make fclean -C libft

.PHONY: fclean
fclean: clean
	rm -f $(NAME)

.PHONY: re
re: fclean all

# rule for testing
test:
	cc $(DEBUGFLAGS) $(SRCFILES) -Llibft -lreadline $(LIBFT) -o $(NAME)

tester:
	cc $(DEBUGFLAGS) tester.c command_utils.c path_utils.c array_utils.c executer.c parser.c lexer.c -Llibft -lreadline $(LIBFT) -o test
