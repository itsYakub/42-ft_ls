#include "./ft_ls.h"

static int ft_part(void *, const size_t, const size_t, const size_t, int (*)(void *, void *));


extern void *ft_qsort(void *arr, const size_t size, const size_t low, const size_t high, int (*compare)(void *, void *)) {
    if (!arr)       { return (0); }
    if (!compare)   { return (0); }

    int part = ft_part(arr, size, low, high, compare);
    if ((size_t) part > low) {
        ft_qsort(arr, size, low, part - 1, compare);
    }
    if ((size_t) part < high) {
        ft_qsort(arr, size, part + 1, high, compare);
    }
    return (arr);
}


static int ft_part(void *arr, const size_t size, const size_t low, const size_t high, int (*compare)(void *, void *)) {
    uint8_t *array = (uint8_t *) arr;    
    uint8_t *pivot = &array[size * high];

    size_t i = low;
    for (size_t j = low; j < high; j++) {
        if (!compare(&array[size * j], pivot)) {
            ft_swap(&array[i * size], &array[j * size], size);
            i++;
        }
    }
    ft_swap(&array[i * size], pivot, size);
    return (i);
}
