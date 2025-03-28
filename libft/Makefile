NAME := libft.a
#TODO remove debug flag
CFLAGS := -g -Wall -Wextra -Werror
SRCDIR := src
OBJDIR := obj

SOURCES := $(shell find $(SRCDIR) -type f -name *.c)
OBJFILES := $(patsubst $(SRCDIR)/%, $(OBJDIR)/%,$(SOURCES:.c=.o))

.PHONY: all
all: $(NAME)

$(NAME): $(OBJFILES)
	ar -r $(NAME) $?

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -I. -o $@ 

.PHONY: clean
clean:
	rm -rf $(OBJDIR)

.PHONY: fclean
fclean: clean
	rm -f libft.a

.PHONY: re
re: fclean all
