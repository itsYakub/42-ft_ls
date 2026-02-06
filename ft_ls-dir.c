#include "./ft_ls.h"


static struct s_file *ft_extract(DIR *, const char *);


static struct s_file *ft_sort(struct s_file *, const size_t);


static int ft_recurse(struct s_file *, const size_t, const char *, t_list **);


static inline int ft_comparea(struct s_file, struct s_file);


/* ft_compared - compare in descending order (alphanum) */
static inline int ft_compared(struct s_file, struct s_file);


/* ft_compareat - compare in ascending order (time) */
static inline int ft_compareat(struct s_file, struct s_file);


/* ft_comparedt - compare in descending order (time) */
static inline int ft_comparedt(struct s_file, struct s_file);


extern struct s_file *ft_process_d(t_list *list) {
    if (!list) { return (0); }

    size_t size = 0;
    struct s_file *files = 0;

    const char *path = list->content;
    DIR *dir = opendir(path);
    if (!dir) {
        return (0);
    }

    size = ft_dircnt(path);
    files = ft_extract(dir, path);
    if (!files) {
        return (0);
    }
    closedir(dir);
  
    files = ft_sort(files, size);
    if (!files) {
        return (0);
    }

    if (g_opt_recursive) {
        ft_recurse(files, size, path, &list);
    }
    
    return (files);
}


static struct s_file *ft_extract(DIR *dir, const char *path) {
    size_t dircnt = ft_dircnt(path);
    struct s_file *files = ft_calloc(dircnt, sizeof(struct s_file));
    if (!files) {
        return (0);
    }

    struct dirent *dirent = 0;
    for (size_t i = 0; (dirent = readdir(dir)); i++) {
        /* create sub-path... */
        char subpath[PATH_MAX + 1] = { 0 };
        ft_strlcat(subpath, path, PATH_MAX);
        if (!ft_strendswith(path, '/')) {
            ft_strlcat(subpath, "/", PATH_MAX);
        }
        ft_strlcat(subpath, dirent->d_name, PATH_MAX);

        struct stat st = { 0 };
        if (stat(subpath, &st) == -1) {
            perror(files[i].f_name);
            free(files); return (0);
        }

        ft_strlcat(files[i].f_name, dirent->d_name, PATH_MAX);
        files[i].f_mode = st.st_mode;
        files[i].f_mtime = st.st_mtime;
        files[i].f_ctime = st.st_ctime;
        files[i].f_atime = st.st_atime;
        files[i].f_uid = st.st_uid;
        files[i].f_gid = st.st_gid;
        files[i].f_nlink = st.st_nlink;
        files[i].f_blkcnt = st.st_blocks;
        files[i].f_dev = st.st_dev;
        files[i].f_rdev = st.st_rdev;
    }

    return (files);
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


static int ft_recurse(struct s_file *arr, const size_t size, const char *path, t_list **list) {
    if (!arr) { return (0); }

    for (size_t i = 0; i < size; i++) {
        /* avoid "cwd" and "pwd" dirents... */
        if (!ft_strcmp(arr[i].f_name, ".") ||
            !ft_strcmp(arr[i].f_name, "..")
        ) {
            continue;
        }

        int mode = arr[i].f_mode & S_IFMT;
        if (mode == S_IFDIR) {
            /* validate '-a' flag... */
            if (*arr[i].f_name == '.') {
                if (!g_opt_all) {
                    continue;
                }
            }

            /* create sub-path... */
            char subpath[PATH_MAX + 1] = { 0 };
            ft_strlcat(subpath, path, PATH_MAX);
            if (!ft_strendswith(path, '/')) {
                ft_strlcat(subpath, "/", PATH_MAX);
            }
            ft_strlcat(subpath, arr[i].f_name, PATH_MAX);

            /* add the new path entry to global paths list... */
            t_list *entry = ft_lstnew(ft_strdup(subpath));
            if (!entry) {
                return (1);
            }

            entry->next = (*list)->next;
            (*list)->next = entry;
            (*list) = (*list)->next;
        }
    }

    return (1);
}


/* ft_comparea - compare in ascending order (alphanum) */
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


/* ft_compared - compare in descending order (alphanum) */
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


/* ft_compareat - compare in ascending order (time) */
static inline int ft_compareat(struct s_file f0, struct s_file f1) {
    size_t t0 = f0.f_mtime;
    size_t t1 = f1.f_mtime;

    if (t0 < t1) { return (1); }
    else if (t0 > t1) { return (0); }
    else {
        return (ft_comparea(f0, f1));
    }
}


/* ft_comparedt - compare in descending order (time) */
static inline int ft_comparedt(struct s_file f0, struct s_file f1) {
    size_t t0 = f0.f_mtime;
    size_t t1 = f1.f_mtime;

    if (t0 > t1) { return (1); }
    else if (t0 < t1) { return (0); }
    else {
        return (ft_compared(f0, f1));
    }
}
