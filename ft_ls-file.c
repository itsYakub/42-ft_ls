#include "./ft_ls.h"


static int ft_extract(struct s_file *, const char *);


static struct s_file *ft_sort(struct s_file *, const size_t);


static inline int ft_comparea(struct s_file, struct s_file);


static inline int ft_compared(struct s_file, struct s_file);


static inline int ft_compareat(struct s_file, struct s_file);


static inline int ft_comparedt(struct s_file, struct s_file);


extern struct s_file *ft_process_f(t_list *list) {
    if (!list) { return (0); }

    size_t size = ft_lstsize(list);
    if (!size) { return (0); }

    struct s_file *arr = ft_calloc(size + 1, sizeof(struct s_file));
    if (!arr) { return (0); }

    for (size_t i = 0, j = 0; i < size; i++, list = list->next) {
        if (ft_extract(&arr[j], list->content)) {
            j++;
        }
    }
    
    arr = ft_sort(arr, size);
    if (!arr) {
        return (0);
    }

    return (arr);
}


static int ft_extract(struct s_file *result, const char *path) {
    if (!path)   { return (0); }
    if (!result) { return (0); }
    
    struct stat st = { 0 };
    if (lstat(path, &st) == -1) {
        return (0);
    }

    ft_strlcat(result->f_name, path, PATH_MAX);
    result->f_size = st.st_size;
    result->f_mode = st.st_mode;
    result->f_mtime = st.st_mtime;
    result->f_mtime = st.st_mtime;
    result->f_ctime = st.st_ctime;
    result->f_atim = st.st_atim;
    result->f_ctim = st.st_ctim;
    result->f_mtim = st.st_mtim;
    result->f_uid = st.st_uid;
    result->f_gid = st.st_gid;
    result->f_nlink = st.st_nlink;
    result->f_blkcnt = st.st_blocks;
    result->f_dev = st.st_dev;
    result->f_rdev = st.st_rdev;

    return (1);
}


static struct s_file *ft_sort(struct s_file *arr, const size_t size) {
    if (!arr) { return (0); }

    int (*compare)(struct s_file, struct s_file) = 0;
    if (g_opt_reverse) {
        if (g_opt_time) {
            compare = ft_comparedt;
        }
        else {
            compare = ft_compared;
        }
    }
    else if (g_opt_time) {
        compare = ft_compareat;
    }
    else {
        compare = ft_comparea;
    }

    return (ft_qsort(arr, 0, size - 1, compare));
}


static inline int ft_comparea(struct s_file f0, struct s_file f1) {
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


static inline int ft_compared(struct s_file f0, struct s_file f1) {
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


static inline int ft_compareat(struct s_file f0, struct s_file f1) {
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


static inline int ft_comparedt(struct s_file f0, struct s_file f1) {
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
