# malloc

subject: https://cdn.intra.42.fr/pdf/pdf/1192/ft_malloc.en.pdf

``void *malloc(size_t size)``: see ``man 3 malloc``

``void *realloc(void *ptr, size_t size)``: see ``man 3 realloc``

``void free(void *ptr)``: see ``man 3 free``

``void *calloc(size_t nbmem, size_t size)``: see ``man 3 calloc``

``void *reallocf(void *ptr, size_t size)``: https://linux.die.net/man/3/reallocf

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

## Environement variables:
|name              |values              |example                        |implemented|description                                        |
|--------------------|------------------|-------------------------------|-----------|---------------------------------------------------|
|MALLOC_SCRIBBLE     |char (hexa or int)|MALLOC_SCRIBBLE=0x42           |no         |use scribble as default value to apply to free data, usefull to see accessed then freed data|
|MALLOC_GUARD_EDGES  |int (hexa or int) |MALLOC_GUARD_EDGES=0x10        |no         |add bytes before and after allocated data as a protection against overflow|
|MALLOC_ALLOC_LOG    |none              |MALLOC_ALLOC_LOG               |no         |print additional information in stdout on alloc or reallocation of memory|
|MALLOC_FREE_LOG     |none              |MALLOC_FREE_LOG                |no         |print additional information in stdout on free or reallocation of memory|
|MALLOC_HEXDUMP      |none              |MALLOC_HEXDUMP                 |no         |change the default behavior of show_alloc_mem()|
|MALLOC_FRAGMENTED   |none              |MALLOC_FRAGMANTED              |no         |do not apply a defragmentation on freed memory, might increase the page mapping, used for debbuging purpose only|
|MALLOC_NO_FREE      |none              |MALLOC_NO_FREE                 |no         |do not set as free freed memory, increase drastically the page mapping, used for debbuging purpose only, free is still simulated, so if combined with log/invalid pointer it will output informations|
|MALLOC_LAZY_ALIGN   |none              |MALLOC_LAZY_ALIGN              |no         |try to free/move invalid pointer if it points to a part of a bloc/list, the original pointer will be used instead of the one passed in parameter of free/realloc, this might result in unwanted free after checking the return of realloc, reallocf should be used instead|
|MALLOC_NO_UNMAP     |none              |MALLOC_NO_UNMAP                |no         |disable all calls to the munmap function, potential speed improvement in numerous alloc/free alternance at cost of hold unused pages|
|MALLOC_EXIT_ON_ERROR|none              |MALLOC_EXIT_ON_ERROR           |no         |explicit, will exit on any error and will force a log of the error|
|MALLOC_LOG_FILE     |path to file      |MALLOC_LOG_FILE="./malloc.log" |no         |will output all log information in the file located at $MALLOC_LOG_FILE location (in append mode) instead of stderr|
