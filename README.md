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
|MALLOC_SCRIBBLE     |char (hexa or int)|MALLOC_SCRIBBLE=0x42           |yes        |use scribble as default value to apply to free data, usefull to see accessed then freed data|
|MALLOC_GUARD_EDGES  |int (hexa or int) |MALLOC_GUARD_EDGES=0x10        |yes        |add bytes before and after allocated data as a protection against overflow|
|MALLOC_ALLOC_LOG    |none              |MALLOC_ALLOC_LOG               |yes        |print additional information in stdout on alloc or reallocation of memory|
|MALLOC_FREE_LOG     |none              |MALLOC_FREE_LOG                |yes        |print additional information in stdout on free or reallocation of memory|
|MALLOC_HEXDUMP      |none              |MALLOC_HEXDUMP                 |no         |change the default behavior of show_alloc_mem()|
|MALLOC_NO_FREE      |none              |MALLOC_NO_FREE                 |yes        |do not set as free freed memory, increase drastically the page mapping, used for debbuging purpose only, free is still simulated, so if combined with log/invalid pointer it will output informations, but will not use the scribble option|
|MALLOC_EXIT_ON_ERROR|none              |MALLOC_EXIT_ON_ERROR           |yes        |explicit, will exit on any error and will force a log of the error|
|MALLOC_LOG_FILE     |path to file      |MALLOC_LOG_FILE="./malloc.log" |yes        |will output all log information in the file located at $MALLOC_LOG_FILE location (in append mode) instead of stderr, by default, if ALLOC_LOG and FREE_LOG arrent set, will only store errors and warnings|
