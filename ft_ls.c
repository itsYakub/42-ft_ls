#include "./ft_ls.h"

/*  ft_ls:
 *  - required flags:
 *      > [ ] -l: use a long listing format
 *      > [X] -R: list subdirectories recursively
 *      > [X] -a: do not ignore entries starting with .
 *      > [X] -r: reverse order while sorting
 *      > [X] -t: sort by time, newest first; see --time (time/date format with -l; see TIME_STYLE below)
 *  - [ ] stylized formatting for regular output (check how ls libft/ behave)"
 *      > columns
 *      > top-to-bottom
 *      > based on length on single line + length of tty line, probably
 *  - [ ] fix the time sorting
 * */

static struct s_file *ft_dirent_process(DIR *, const char *, t_list *);


static struct s_file *ft_file_sort(struct s_file *, const size_t);


static int ft_file_print(struct s_file *, const size_t);


static int ft_file_recursive_enter(struct s_file *, const size_t, const char *, t_list **);


/* ft_file_comparea - compare in ascending order (alphanum) */
static inline int ft_file_comparea(struct s_file, struct s_file);


/* ft_file_compared - compare in descending order (alphanum) */
static inline int ft_file_compared(struct s_file, struct s_file);


/* ft_file_compareat - compare in ascending order (time) */
static inline int ft_file_compareat(struct s_file, struct s_file);


/* ft_file_comparedt - compare in descending order (time) */
static inline int ft_file_comparedt(struct s_file, struct s_file);


int main(int ac, char **av) {
    int exitcode = 0;

    /* process command-line arguments... */
    int getopt = ft_getopt(ac, av);
    if (getopt != 0) {
        exitcode = getopt; goto main_exit;
    }

    /* set default starting path if needed... */
    if (!g_paths || !ft_lstsize(g_paths)) {
        g_paths = ft_lstnew(ft_strdup("."));
        if (!g_paths) {
            exitcode = 1; goto main_exit;
        }
    }

    for (t_list *path = g_paths; path; path = path->next) {
        struct s_file *files = ft_process_subdirs(path->content, path);
        if (!files) {
            exitcode = 1; goto main_exit;
        }

        /* we need to recalculate size each iteration (recursive flag can change the g_paths length)... */
        size_t lstsize = ft_lstsize(g_paths);
        if (lstsize > 1) {
            /* put '\n' between each entry but not before the initial 'path'... */
            if (path != g_paths) {
                ft_putchar_fd(10, 1);
            }

            ft_putstr_fd(path->content, 1);
            ft_putendl_fd(":", 1);
        }
        ft_file_print(files, ft_dircnt(path->content));
        
        free(files), files = 0;
    }


main_exit:

    if (g_paths) { ft_lstclear(&g_paths, free), g_paths = 0; }

    return (exitcode);
}


extern struct s_file *ft_process_subdirs(const char *path, t_list *list) {
    if (!path) { return (0); }

    struct s_file *files = 0;
    DIR *dir = opendir(path);
    /* process individual files (check: ls ft_ls)... */
    if (!dir) {
        /* ... */
        return (0);
    }
    /* process directory entries (check: ls .)... */
    else {
        files = ft_dirent_process(dir, path, list);
        if (!files) {
            return (0);
        }
    }
    closedir(dir);
    
    return (files);
}


static struct s_file *ft_dirent_process(DIR *dir, const char *path, t_list *list) {
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
    
    files = ft_file_sort(files, dircnt);
    if (!files) {
        return (0);
    }

    if (g_opt_recursive) {
        ft_file_recursive_enter(files, dircnt, path, &list);
    }

    return (files);
}


static struct s_file *ft_file_sort(struct s_file *arr, const size_t size) {
    if (!arr) { return (0); }

    int (*compare)(struct s_file, struct s_file) = 0;
    if (g_opt_reverse) {
        if (g_opt_time) {
            compare = ft_file_comparedt;
        }
        else {
            compare = ft_file_compared;
        }
    }
    else if (g_opt_time) {
        compare = ft_file_compareat;
    }
    else {
        compare = ft_file_comparea;
    }

    return (ft_file_qsort(arr, 0, size - 1, compare));
}


static int ft_file_print(struct s_file *arr, const size_t size) {
    if (!arr) { return (0); }

    int status = 0;
    /* validate '-l' flag... */
    if (!g_opt_list) {
        status = ft_print_vertical(arr, size);
    }
    else {
        /* ... */
    }
    
    return (status);
}


static int ft_file_recursive_enter(struct s_file *arr, const size_t size, const char *path, t_list **list) {
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


/* ft_file_comparea - compare in ascending order (alphanum) */
static inline int ft_file_comparea(struct s_file f0, struct s_file f1) {
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


/* ft_file_compared - compare in descending order (alphanum) */
static inline int ft_file_compared(struct s_file f0, struct s_file f1) {
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


/* ft_file_compareat - compare in ascending order (time) */
static inline int ft_file_compareat(struct s_file f0, struct s_file f1) {
    size_t t0 = f0.f_mtime;
    size_t t1 = f1.f_mtime;

    if (t0 < t1) { return (1); }
    else if (t0 > t1) { return (0); }
    else {
        return (ft_file_comparea(f0, f1));
    }
}


/* ft_file_comparedt - compare in descending order (time) */
static inline int ft_file_comparedt(struct s_file f0, struct s_file f1) {
    size_t t0 = f0.f_mtime;
    size_t t1 = f1.f_mtime;

    if (t0 > t1) { return (1); }
    else if (t0 < t1) { return (0); }
    else {
        return (ft_file_compared(f0, f1));
    }
}


/* g_opt_list - output in list format (check: ls -l)
 * */
int g_opt_list = 0;

/* g_opt_recursive - recursively enter to subdirectories (check: ls -R)
 * */
int g_opt_recursive = 0;

/* g_opt_all - output all files and directories (including hidden) (check: ls -a)
 * */
int g_opt_all = 0;

/* g_opt_reverse - sort entries in descending alphanumerical order (check: ls -r)
 * */
int g_opt_reverse = 0;

/* g_opt_time - sort entries by newest first (check: ls -t)
 * */
int g_opt_time = 0;

/* g_paths - global linked list of processed paths (default: '.')
 * */
t_list *g_paths = 0;

/* g_prog - name of the executable (av[0])
 * */
const char *g_prog = 0;
