NAME :=	malloc.so

OBJ :=	obj/malloc.o\
		obj/realloc.o\
		obj/free.o\
		obj/malloc_init.o\
		obj/show_alloc_mem.o

all: $(NAME)

$(NAME): $(OBJ)
	gcc -Wall -Wextra -Werror -shared -o $@ $^

obj/%.o: src/%.c inc/malloc.h Makefile
	@mkdir -p obj
	gcc -Wall -Wextra -Werror -I inc -o $@ -c $<

clean:
	rm -rf obj

fclean: clean
	rm -f $(NAME)

re: fclean all
