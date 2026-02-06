#include "./ft_ls.h"

static int ft_partition(struct s_file *, const size_t, const size_t, int (*)(struct s_file, struct s_file));


extern struct s_file *ft_file_qsort(struct s_file *arr, const size_t low, const size_t high, int (*compare)(struct s_file, struct s_file)) {
    if (!arr)       { return (0); }
    if (!compare)   { return (0); }
    
    int partition = ft_partition(arr, low, high, compare);
    if ((size_t) partition > low) {
        ft_file_qsort(arr, low, partition - 1, compare);
    }
    if ((size_t) partition < high) {
        ft_file_qsort(arr, partition + 1, high, compare);
    }
    return (arr);
}


static int ft_partition(struct s_file *arr, const size_t low, const size_t high, int (*compare)(struct s_file, struct s_file)) {
    struct s_file pivot = arr[high];

    size_t i = low;
    for (size_t j = low; j < high; j++) {
        if (!compare(arr[j], pivot)) {
            struct s_file tmp0 = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp0;
            i++;
        }
    }
    struct s_file tmp0 = arr[i];
    arr[i] = arr[high];
    arr[high] = tmp0;
    return (i);
}
