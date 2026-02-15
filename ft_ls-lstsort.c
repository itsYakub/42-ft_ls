#include "./ft_ls.h"

extern t_list *ft_lstsort(t_list *list) {
    int (*compare)(void *, void *) = 0;
    if (g_opt_reverse) {
        if (g_opt_time) {
            compare = ft_compareldt;
        }
        else {
            compare = ft_compareld;
        }
    }
    else if (g_opt_time) {
        compare = ft_comparelat;
    }
    else {
        compare = ft_comparela;
    }

    size_t size = ft_lstsize(list);
    char **arr = ft_calloc(size, sizeof(char *));
    if (!arr) {
        return (list);
    }

    size_t i = 0;
    for (t_list *item = list; item; item = item->next) {
        arr[i++] = item->content; 
    }

    arr = ft_qsort(arr, sizeof(*arr), 0, size - 1, compare);

    i = 0;
    for (t_list *item = list; item; item = item->next) {
        item->content = arr[i++];
    }
    free(arr);

    return (list);
}


extern int ft_comparela(void *v0, void *v1) {
    char *n0 = *((char **) v0);
    char *n1 = *((char **) v1);
    
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


extern int ft_compareld(void *v0, void *v1) {
    char *n0 = *((char **) v0);
    char *n1 = *((char **) v1);

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


extern int ft_comparelat(void *v0, void *v1) {
    const char *n0 = (const char *) v0;
    struct stat st0 = { 0 };
    lstat(n0, &st0);

    const char *n1 = (const char *) v1;
    struct stat st1 = { 0 };
    lstat(n1, &st1);
    
    size_t t0 = st0.st_mtim.tv_sec;
    size_t t1 = st1.st_mtim.tv_sec;

    if (t0 < t1) { return (1); }
    else if (t0 > t1) { return (0); }
    
    t0 = st0.st_mtim.tv_nsec;
    t1 = st1.st_mtim.tv_nsec;
    if (t0 < t1) { return (1); }
    else if (t0 > t1) { return (0); }
    else {
        return (ft_comparela(v0, v1));
    }
}


extern int ft_compareldt(void *v0, void *v1) {
    const char *n0 = (const char *) v0;
    struct stat st0 = { 0 };
    lstat(n0, &st0);

    const char *n1 = (const char *) v1;
    struct stat st1 = { 0 };
    lstat(n1, &st1);
    
    size_t t0 = st0.st_mtim.tv_sec;
    size_t t1 = st1.st_mtim.tv_sec;

    if (t0 > t1) { return (1); }
    else if (t0 < t1) { return (0); }
    
    t0 = st0.st_mtim.tv_nsec;
    t1 = st1.st_mtim.tv_nsec;
    if (t0 > t1) { return (1); }
    else if (t0 < t1) { return (0); }
    else {
        return (ft_compareld(v0, v1));
    }
}
