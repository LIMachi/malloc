NAME :=	libft_malloc.so

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

clean_tests:
	rm -f test/test0 test/test1 test/test2 test/test3 test/test3++ test/test4 test/test5

test_no_malloc_system:
	clear
	cat test/test0.c
	gcc -o test/test0 test/test0.c
	/usr/bin/time -l test/test0

test_malloc_system:
	clear
	cat test/test1.c
	gcc -o test/test1 test/test1.c
	/usr/bin/time -l test/test1

test_no_malloc_user:
	clear
	cat test/run.sh
	cat test/test0.c
	gcc -o test/test0 test/test0.c
	test/run.sh /usr/bin/time -l test/test0

test_malloc_user:
	clear
	cat test/run.sh
	cat test/test1.c
	gcc -o test/test1 test/test1.c
	test/run.sh /usr/bin/time -l test/test1

test_free_system:
	clear
	cat test/run.sh
	cat test/test2.c
	gcc -o test/test2 test/test2.c
	/usr/bin/time -l test/test2

test_free_user:
	clear
	cat test/run.sh
	cat test/test2.c
	gcc -o test/test2 test/test2.c
	test/run.sh /usr/bin/time -l test/test2

test_realloc_user:
	clear
	cat test/run.sh
	cat test/test3.c
	gcc -o test/test3 test/test3.c
	test/run.sh test/test3

test_realloc++:
	clear
	cat test/run.sh
	cat test/test3++.c
	gcc -o test/test3++ test/test3++.c
	test/run.sh test/test3++

test_errors:
	clear
	cat test/run.sh
	cat test/test4.c
	gcc -o test/test4 test/test4.c
	test/run.sh test/test4

test_show_alloc_mem:
	clear
	cat test/test5.c
	gcc -o test/test5 test/test5.c
	test/test5
