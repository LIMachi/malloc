ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

ifeq ($(BONUS),)
	BONUS := 0
endif


ifeq ($(shell uname), Linux)
TIME := /usr/bin/time -v
TEST_SH := test/run_linux.sh
else
TIME := /usr/bin/time -l
TEST_SH := test/run_mac.sh
endif


HNAME := libft_malloc_$(HOSTTYPE).so
NAME := libft_malloc.so

OBJ :=	obj/malloc.o\
		obj/init.o\
		obj/ma_get_space.o\
		obj/ma_new_pool.o\
		obj/categorize.o\
		obj/ma_debug_itoabuff.o\
		obj/ma_validate_pointer.o\
		obj/reallocf.o\
		obj/realloc.o\
		obj/free.o\
		obj/show_alloc_mem.o\
		obj/calloc.o\
		obj/ma_next_block.o\
		obj/malloc_trim.o\
		obj/malloc_usable_size.o\
		obj/ma_log.o

FLAGS := -Wall -Wextra -Werror -g -fPIC #-DBONUS=$(BONUS) -rdynamic

all: $(NAME)

$(NAME): $(HNAME)
	rm -f $@
	ln -s $< $@

$(HNAME): $(OBJ)
	gcc $(FLAGS) -shared -o $@ $^

obj/%.o: src/%.c inc/malloc.h Makefile
	@mkdir -p obj
	gcc $(FLAGS) -I inc -o $@ -c $<

clean:
	rm -rf obj

fclean: clean clean_tests
	rm -f $(NAME) $(HNAME)

re: fclean all

clean_tests:
	rm -f test/test0 test/test1 test/test2 test/test3 test/test3++ test/test4 test/test5 test/test6

norm:
	/Users/hmartzol/.bin/norminette.sh inc src

test_no_malloc_system:
	clear
	cat test/test0.c
	gcc -o test/test0 test/test0.c
	$(TIME) test/test0

test_malloc_system:
	clear
	cat test/test1.c
	gcc -o test/test1 test/test1.c
	$(TIME) test/test1

test_no_malloc_user: $(NAME)
	clear
	cat $(TEST_SH)
	cat test/test0.c
	gcc -o test/test0 test/test0.c -L. -lft_malloc
	$(TEST_SH) $(TIME) test/test0

test_malloc_user: $(NAME)
	clear
	cat $(TEST_SH)
	cat test/test1.c
	gcc -o test/test1 test/test1.c -L. -lft_malloc
	$(TEST_SH) $(TIME) test/test1

test_free_system:
	clear
	cat $(TEST_SH)
	cat test/test2.c
	gcc -o test/test2 test/test2.c
	$(TIME) test/test2

test_free_user: $(NAME)
	clear
	cat $(TEST_SH)
	cat test/test2.c
	gcc -o test/test2 test/test2.c -L. -lft_malloc
	$(TEST_SH) $(TIME) test/test2

test_realloc_user: $(NAME)
	clear
	cat $(TEST_SH)
	cat test/test3.c
	gcc -o test/test3 test/test3.c -L. -lft_malloc
	$(TEST_SH) test/test3

test_realloc++: $(NAME)
	clear
	cat $(TEST_SH)
	cat test/test3++.c
	gcc -o test/test3++ test/test3++.c -L. -lft_malloc
	$(TEST_SH) test/test3++

test_errors: $(NAME)
	clear
	cat $(TEST_SH)
	cat test/test4.c
	gcc -o test/test4 test/test4.c -L. -lft_malloc
	$(TEST_SH) test/test4

test_show_alloc_mem: $(NAME)
	clear
	cat test/test5.c
	gcc -o test/test5 test/test5.c -L. -lft_malloc
	$(TEST_SH) test/test5

test_this_is_madness: $(NAME)
	clear
	cat $(TEST_SH)
	echo "dare use vim now :p"
	$(TEST_SH) /bin/bash

test_alternative_method_system: $(NAME)
	clear
	cat test/test6.c
	gcc -o test/test6 test/test6.c
	test/test6

test_alternative_method_user: $(NAME)
	clear
	cat $(TEST_SH)
	cat test/test6.c
	gcc -o test/test6 test/test6.c -L. -lft_malloc
	$(TEST_SH) test/test6
