# malloc

subject: https://cdn.intra.42.fr/pdf/pdf/1192/ft_malloc.en.pdf

``void *malloc(size_t size)``: see ``man 3 malloc``

``void *realloc(void *ptr, size_t size)``: see ``man 3 realloc``

``void free(void *ptr)``: see ``man 3 free``

``void *calloc(size_t nbmem, size_t size)``: see ``man 3 calloc``

``void *reallocf(void *ptr, size_t size)``: same as ``realloc`` but will ``free(ptr)`` if the return is not ``ptr`` itself (NULL or memory was relocated)

``void show_alloc_mem(void)``: print information on allocated memory in the form:
```
TINY : 0xA0000
0xA0020 - 0xA004A : 42 bytes
0xA006A - 0xA00BE : 84 bytes
SMALL : 0xAD000
0xAD020 - 0xADEAD : 3725 bytes
LARGE : 0xB0000
0xB0020 - 0xBBEEF : 48847 bytes
Total : 52698 octets
```

# Method used
Two (or three if we count the special treatment to large allocations) methods where used for this project, one by default and one activable in bonus mode.
The method consist of cutting pages in blocs of set size and allocating (using a system of flags) one or more blocs to contain ``size`` bytes.
To keep track of used pages and blocs, one or more pages will be used as a header for multiple pages of data, the structure of those page is roughly:
```c
struct                            s_ma_header_small_tiny
{
  struct s_ma_header_small_tiny   *next;
  struct s_ma_header_small_tiny   *prev;
  void                            *pages[pages_per_header];
  uint16_t                        usage[pages_per_header][blocs_per_page];
}
```
An example of allocation might be that I requested 42 bytes and the size of blocs in a tiny page is 32 bytes, then i will need 2 contiguous blocs in a page, I found the 42nd bloc of the 2nd page of the 1st group of pages:
```c
size_t  page = 2;
size_t  bloc = 41;
group->usage[page][bloc] = id;
group->usage[page][bloc + 1] = id;
return (group->pages[page] + size_of_bloc * bloc);
```
In the case of large allocation (bigger than a single page), a group of contiguous pages will be mapped and a structure of the form
```
typedef struct              s_ma_header_large
{
  struct s_ma_header_large  *next;
  struct s_ma_header_large  *prev;
  size_t                    size;
  char                      data[0];
}                           t_ma_header_large;
```
will be used as a header

# Bonus
You can activate the bonus mode by compiling with the define BONUS set to 1, or by adding BONUS=1 in the parameters of make
Compiling the bonuses add thread safety for pthread, use of errno to pass error information and a bunch of option to pass to the executables created with this library

## List mode
Alternative mode of paging and storing of data for small and tiny allocations
Instead of cutting pages by blocs, pages will containt double linked list of allocations in the form:
```
page bytes:
  next page ptr
  previous page ptr
  size
  allocation:
    next allocation ptr
    previous allocation ptr
    size (includes status in the low bit)
    data
```

## Environement variables:
|name              |values            |example                |bloc mode|list mode|description                                        |
|------------------|------------------|-----------------------|---------|---------|---------------------------------------------------|
|MALLOC_MODE       |LIST/BLOC/DEFAULT |MALLOC_MODE=LIST       |yes      |yes      |switch mode                                        |
|MALLOC_SCRIBBLE   |char (hexa or int)|MALLOC_SCRIBBLE=0x42   |yes      |yes      |use scribble as default value to apply to free data, usefull to see accessed then freed data|
|MALLOC_GUARD_EDGES|int (hexa or int) |MALLOC_GUARD_EDGES=0x10|yes      |yes      |add bytes before and after allocated data as a protection against overflow|
|MALLOC_ALLOC_LOG  |none              |MALLOC_ALLOC_LOG       |yes      |yes      |print additional information in stdout on alloc or reallocation of memory|
|MALLOC_FREE_LOG   |none              |MALLOC_FREE_LOG        |yes      |yes      |print additional information in stdout on free or reallocation of memory|
|MALLOC_HEXDUMP    |none              |MALLOC_HEXDUMP         |yes      |yes      |change the default behavior of ``show_alloc_mem()``|
|MALLOC_FRAGMENTED |none              |MALLOC_FRAGMANTED      |no       |yes      |do not apply a defragmentation on freed memory, might increase the page mapping, used for debbuging purpose only|
|MALLOC_NO_FREE    |none              |MALLOC_NO_FREE         |yes      |yes      |do not set as free freed memory, increase drastically the page mapping, used for debbuging purpose only|
