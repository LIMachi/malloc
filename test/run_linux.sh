#!/bin/sh
export LD_LIBRARY_PATH=.
export LD_INSERT_LIBRARIES="libft_malloc.so"
export LD_FORCE_FLAT_NAMESPACE=1
export LD_PRELOAD="libft_malloc.so"
$@
