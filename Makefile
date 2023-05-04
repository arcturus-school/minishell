NAME  = minishell
UTILS = utils/*.c
SRC   = src/*.c

build:
	gcc -o $(NAME) main.c $(UTILS) $(SRC) 

test:
	./$(NAME)

clean:
	rm -f $(NAME)