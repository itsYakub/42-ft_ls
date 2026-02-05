#include "./ft_ls.h"

static int ft_partition(struct dirent **, const size_t, const size_t, int (*)(struct dirent *, struct dirent *));


extern struct dirent **ft_dirent_qsort(struct dirent **arr, const size_t low, const size_t high, int (*compare)(struct dirent *, struct dirent *)) {
    if (!arr)       { return (0); }
    if (!compare)   { return (0); }
    
    int partition = ft_partition(arr, low, high, compare);
    if ((size_t) partition > low) {
        ft_dirent_qsort(arr, low, partition - 1, compare);
    }
    if ((size_t) partition < high) {
        ft_dirent_qsort(arr, partition + 1, high, compare);
    }
    return (arr);
}


static int ft_partition(struct dirent **arr, const size_t low, const size_t high, int (*compare)(struct dirent *, struct dirent *)) {
    struct dirent *pivot = arr[high];

    size_t i = low;
    for (size_t j = low; j < high; j++) {
        if (!compare(arr[j], pivot)) {
            struct dirent *tmp0 = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp0;
            i++;
        }
    }
    struct dirent *tmp0 = arr[i];
    arr[i] = arr[high];
    arr[high] = tmp0;
    return (i);
}
