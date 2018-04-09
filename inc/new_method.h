4096

head {		16
	prev	8
	next	8
}
pages_pointers { pages * 8
	pages 8
}
blocs_tiny (16) { pages * 512
	usage 256 * pages
	owner 256 * pages
}
blocs_small (128) { pages * 64
	usage 32 * pages
	owner 32 * pages
}

total tiny: pages * 520 + 16 == 4096 ~= pages == 7 loss: 440
total small: pages * 72 + 16 == 4096 ~= pages == 56 loss: 48

blocs_per_pages = 4096 / BLOC_SIZE
first alloc = seil(128 / blocs_per_pages) * pages
bloc_head = 8 + blocs_per_pages * 2
pages == (int)(4080 / bloc_head)
loss == 4080 - pages * bloc_head

1024 * malloc(1024) == aprox 5 header plus 256 small pages, 261 pages total

search of pointer:
first rule: never dereference a pointer given to free/realloc
test if pointer is in a page by comparing the pointer and the page itself (if ((size_t)pointer >= (size_t)page[n] && (size_t)pointer < pagesize + (size_t)page[n]))
since all allocated blocs are aligned on BLOC_SIZE, non aligned pointer must be refused (if (!((size_t)pointer - (size_t)page[n]) % BLOC_SIZE)))
finally, we can push the security further by testing if previous bloc are allocated to the same group
since all mmaped pages are never munmaped, pages_pointers are list of up to x pages and eventually NULL terminated (if less than x)
