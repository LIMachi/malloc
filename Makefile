ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

ifeq ($(BONUS),)
	BONUS := 0
endif

HNAME := libft_malloc_$(HOSTTYPE).so
NAME := libft_malloc.so

OBJ :=	obj/malloc.o\
		obj/realloc.o\
		obj/free.o\
		obj/malloc_init.o\
		obj/show_alloc_mem.o

all: $(NAME)

$(NAME): $(HNAME)
	ln -Fs $< $@

$(HNAME): $(OBJ)
	gcc -Wall -Wextra -Werror -DBONUS=$(BONUS) -shared -o $@ $^

obj/%.o: src/%.c inc/malloc.h Makefile
	@mkdir -p obj
	gcc -Wall -Wextra -Werror -DBONUS=$(BONUS) -I inc -o $@ -c $<

clean:
	rm -rf obj

fclean: clean clean_tests
	rm -f $(NAME) $(HNAME)

re: fclean all

clean_tests:
	rm -f test/test0 test/test1 test/test2 test/test3 test/test3++ test/test4 test/test5

norm:
	/Users/hmartzol/.bin/norminette.sh inc src

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

test_no_malloc_user: $(NAME)
	clear
	cat test/run.sh
	cat test/test0.c
	gcc -o test/test0 test/test0.c
	test/run.sh /usr/bin/time -l test/test0

test_malloc_user: $(NAME)
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

test_free_user: $(NAME)
	clear
	cat test/run.sh
	cat test/test2.c
	gcc -o test/test2 test/test2.c
	test/run.sh /usr/bin/time -l test/test2

test_realloc_user: $(NAME)
	clear
	cat test/run.sh
	cat test/test3.c
	gcc -o test/test3 test/test3.c
	test/run.sh test/test3

test_realloc++: $(NAME)
	clear
	cat test/run.sh
	cat test/test3++.c
	gcc -o test/test3++ test/test3++.c
	test/run.sh test/test3++

test_errors: $(NAME)
	clear
	cat test/run.sh
	cat test/test4.c
	gcc -o test/test4 test/test4.c
	test/run.sh test/test4

test_show_alloc_mem: $(NAME)
	clear
	cat test/test5.c
	gcc -o test/test5 test/test5.c
	test/test5
