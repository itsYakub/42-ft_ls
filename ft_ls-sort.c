#include "./ft_ls.h"

static int ft_part(void *, const size_t, const size_t, const size_t, int (*)(void *, void *));


extern struct s_file *ft_qsort(struct s_file *arr, const size_t size, const size_t low, const size_t high, int (*compare)(void *, void *)) {
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


extern int ft_comparefa(void *f0, void *f1) {
    const char *n0 = ((struct s_file *) f0)->f_name;
    const char *n1 = ((struct s_file *) f1)->f_name;
    
    /* special case: n0 == "..", n1 == "." */
    if (!ft_strcmp(n0, "..") &&
        !ft_strcmp(n1, ".")
    ) {
        return (1);
    }

    while (*n0 || *n1) {
        while (*n0 && !ft_isalnum(*n0)) { n0++; }
        while (*n1 && !ft_isalnum(*n1)) { n1++; }
        if (ft_tolower(*n0) != ft_tolower(*n1)) {
            return (ft_tolower(*n0) > ft_tolower(*n1));
        }

        if (*n0) { n0++; }
        if (*n1) { n1++; }
    }

    return (ft_strcmp(n0, n1) < 0);
}


extern int ft_comparefd(void *f0, void *f1) {
    const char *n0 = ((struct s_file *) f0)->f_name;
    const char *n1 = ((struct s_file *) f1)->f_name;

    /* special case: n0 == ".", n1 == ".." */
    if (!ft_strcmp(n0, ".") &&
        !ft_strcmp(n1, "..")
    ) {
        return (1);
    }

    while (*n0 || *n1) {
        while (*n0 && !ft_isalnum(*n0)) { n0++; }
        while (*n1 && !ft_isalnum(*n1)) { n1++; }
        if (ft_tolower(*n0) != ft_tolower(*n1)) {
            return (ft_tolower(*n0) < ft_tolower(*n1));
        }

        if (*n0) { n0++; }
        if (*n1) { n1++; }
    }

    return (ft_strcmp(n0, n1) > 0);
}


extern int ft_comparefat(void *f0, void *f1) {
    size_t t0 = ((struct s_file *) f0)->f_mtim.tv_sec;
    size_t t1 = ((struct s_file *) f1)->f_mtim.tv_sec;

    if (t0 < t1) { return (1); }
    else if (t0 > t1) { return (0); }
    
    t0 = ((struct s_file *) f0)->f_mtim.tv_nsec;
    t1 = ((struct s_file *) f1)->f_mtim.tv_nsec;
    if (t0 < t1) { return (1); }
    else if (t0 > t1) { return (0); }
    else {
        return (ft_comparefa(f0, f1));
    }
}


extern int ft_comparefdt(void *f0, void *f1) {
    size_t t0 = ((struct s_file *) f0)->f_mtim.tv_sec;
    size_t t1 = ((struct s_file *) f1)->f_mtim.tv_sec;

    if (t0 > t1) { return (1); }
    else if (t0 < t1) { return (0); }
    
    t0 = ((struct s_file *) f0)->f_mtim.tv_nsec;
    t1 = ((struct s_file *) f1)->f_mtim.tv_nsec;
    if (t0 > t1) { return (1); }
    else if (t0 < t1) { return (0); }
    else {
        return (ft_comparefd(f0, f1));
    }
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
    ft_swap(&array[i * size], &array[high * size], size);
    return (i);
}
