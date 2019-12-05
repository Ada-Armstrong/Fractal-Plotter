CC= gcc
CFLAGS= -Wall -Wextra -Werror -pedantic -Ofast
LIB= -lpthread
OBJ= main.o arg.o frame.o ppm.o tokenize.o
NAME= cfp

$(NAME): $(OBJ)
	$(CC) -o $@ $(OBJ) $(CFLAGS) $(LIB)

clean:
	rm $(NAME) $(OBJ)
