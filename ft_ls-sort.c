#include "./ft_ls.h"

static int ft_part(struct s_file *, const size_t, const size_t, int (*)(struct s_file, struct s_file));


extern struct s_file *ft_qsort(struct s_file *arr, const size_t low, const size_t high, int (*compare)(struct s_file, struct s_file)) {
    if (!arr)       { return (0); }
    if (!compare)   { return (0); }
    
    int part = ft_part(arr, low, high, compare);
    if ((size_t) part > low) {
        ft_qsort(arr, low, part - 1, compare);
    }
    if ((size_t) part < high) {
        ft_qsort(arr, part + 1, high, compare);
    }
    return (arr);
}


extern int ft_comparea(struct s_file f0, struct s_file f1) {
    const char *n0 = f0.f_name;
    const char *n1 = f1.f_name;
    
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


extern int ft_compared(struct s_file f0, struct s_file f1) {
    const char *n0 = f0.f_name;
    const char *n1 = f1.f_name;

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


extern int ft_compareat(struct s_file f0, struct s_file f1) {
    size_t t0 = f0.f_mtim.tv_sec;
    size_t t1 = f1.f_mtim.tv_sec;

    if (t0 < t1) { return (1); }
    else if (t0 > t1) { return (0); }
    
    t0 = f0.f_mtim.tv_nsec;
    t1 = f1.f_mtim.tv_nsec;
    if (t0 < t1) { return (1); }
    else if (t0 > t1) { return (0); }
    else {
        return (ft_comparea(f0, f1));
    }
}


extern int ft_comparedt(struct s_file f0, struct s_file f1) {
    size_t t0 = f0.f_mtim.tv_sec;
    size_t t1 = f1.f_mtim.tv_sec;

    if (t0 > t1) { return (1); }
    else if (t0 < t1) { return (0); }
    
    t0 = f0.f_mtim.tv_nsec;
    t1 = f1.f_mtim.tv_nsec;
    if (t0 > t1) { return (1); }
    else if (t0 < t1) { return (0); }
    else {
        return (ft_compared(f0, f1));
    }
}


static int ft_part(struct s_file *arr, const size_t low, const size_t high, int (*compare)(struct s_file, struct s_file)) {
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
