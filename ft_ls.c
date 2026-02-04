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
 *  - [ ] replace bubble sort with quick sort (cmon man)
 * */

static struct dirent **ft_dirent_sort(struct dirent **, const size_t);


static char *ft_dirent_print(struct dirent **);


static char *ft_dirent_recursive_enter(struct dirent **, const char *);


/* ft_dirent_comparea - compare in ascending order (alphanum) */
static inline int ft_dirent_comparea(struct dirent *, struct dirent *);


/* ft_dirent_compared - compare in descending order (alphanum) */
static inline int ft_dirent_compared(struct dirent *, struct dirent *);


/* ft_dirent_compareat - compare in ascending order (time) */
static inline int ft_dirent_compareat(struct dirent *, struct dirent *);


/* ft_dirent_comparedt - compare in descending order (time) */
static inline int ft_dirent_comparedt(struct dirent *, struct dirent *);


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
 
    char *output = 0;
    for (t_list *path = g_paths; path; path = path->next) {
        char *tmp0 = ft_process_subdirs(path->content);
        if (!tmp0) {
            exitcode = 1; goto main_exit;
        }

        output = output == 0 ?
            ft_strdup(tmp0) :
            ft_strjoin_free(output, tmp0);

        free(tmp0), tmp0 = 0;
    }

    ft_putstr_fd(output, 1);

main_exit:

    if (output)  { free(output), output = 0; }
    if (g_paths) { ft_lstclear(&g_paths, free), g_paths = 0; }

    return (exitcode);
}


extern char *ft_process_subdirs(const char *path) {
    if (!path) { return (0); }

    /* 1. open directory on path 'path->content'... */
    DIR *dir = opendir(path);
    if (!dir) {
        return (0);
    }

    /* 2. allocate an array for all directory entries... */
    size_t dirndx = 0;
    size_t dircnt = ft_dircnt(path);
    struct dirent **dirents = calloc(dircnt + 1, sizeof(struct dirent *));
    if (!dirents) {
        closedir(dir);
        return (0);
    }

    /* 3. extract every directory entry to dirents... */
    struct dirent *dirent = 0;
    while ((dirent = readdir(dir))) {
        dirents[dirndx++] = dirent;
    }
    
    /* 4. process directory entries... */

    /* 4.1. sort entries... */
    dirents = ft_dirent_sort(dirents, dircnt);
    if (!dirents) {
        return (0);
    }

    /* 4.2. insert every name to output string */
    char *output = ft_dirent_print(dirents);

    /* 4.3. process subdirectories recursively... */

    /* validate '-R' flag... */
    if (g_opt_recursive) {
        char *tmp0 = ft_dirent_recursive_enter(dirents, path);
        if (tmp0) {
            output = ft_strjoin_free(output, "\n");
            output = ft_strjoin_free(output, tmp0);
            free(tmp0), tmp0 = 0;
        }
    }

    /* 5. cleanup... */
    closedir(dir);
    free(dirents);

    return (output);
}


static struct dirent **ft_dirent_sort(struct dirent **arr, const size_t count) {
    if (!arr) { return (0); }

    int (*compare)(struct dirent *, struct dirent *) = 0;
    if (g_opt_reverse) {
        if (g_opt_time) {
            compare = ft_dirent_comparedt;
        }
        else {
            compare = ft_dirent_compared;
        }
    }
    else if (g_opt_time) {
        compare = ft_dirent_compareat;
    }
    else {
        compare = ft_dirent_comparea;
    }

    for (size_t i = 0; i < count - 1; i++) {
        for (size_t j = 0; j < count - 1 - i; j++) {
            struct dirent *d0 = arr[j];
            struct dirent *d1 = arr[j + 1];

            if (compare(d0, d1)) {
                arr[j] = d1;
                arr[j + 1] = d0;
            }
        }
    }

    return (arr);
}


static char *ft_dirent_print(struct dirent **dirents) {
    if (!dirents) { return (0); }

    char *output = 0;
    for (size_t i = 0; dirents[i]; i++) {
        struct dirent dirent = *dirents[i];
                
        /* validate '-a' flag... */
        if (*dirent.d_name == '.') {
            if (!g_opt_all) {
                continue;
            }
        }
       
        /* validate '-l' flag... */
        if (!g_opt_list) {
            output = output == 0 ?
                ft_strdup(dirent.d_name) :
                ft_strjoin_free(output, dirent.d_name);

            output = ft_strjoin_free(output, "  ");
        }
        else {
            /* formatted list output... */
        }
    }
    
    if (!g_opt_list) {
        output = ft_strjoin_free(output, "\n");
    }
    return (output);
}


static char *ft_dirent_recursive_enter(struct dirent **dirents, const char *path) {
    if (!dirents) { return (0); }

    char *output = 0;
    for (size_t i = 0; dirents[i]; i++) {
        struct dirent dirent = *dirents[i];

        /* avoid "cwd" and "pwd" dirents... */
        if (!ft_strcmp(dirent.d_name, ".") ||
            !ft_strcmp(dirent.d_name, "..")
        ) {
            continue;
        }

        if (dirent.d_type == DT_DIR) {

            /* validate '-a' flag... */
            if (*dirent.d_name == '.') {
                if (!g_opt_all) {
                    continue;
                }
            }

            /* create sub-path... */
            char *subpath = 0;

            /* ...also, check if supplied path ends with '/'; if not, append it first... */
            if (!ft_strendswith(path, '/')) {
                subpath = ft_strjoin(path, "/");
                subpath = ft_strjoin_free(subpath, dirent.d_name);
            }
            else {
                subpath = ft_strjoin(path, dirent.d_name);
            }

            char *tmp0 = ft_process_subdirs(subpath);
            if (!tmp0) {
                free(subpath), subpath = 0; continue;
            }

            output = output == 0 ?
                ft_strdup(subpath) :
                ft_strjoin_free(output, subpath);

            output = ft_strjoin_free(output, ":\n");
            output = ft_strjoin_free(output, tmp0);
            
            free(subpath), subpath = 0;
            free(tmp0), tmp0 = 0;
        }
    }

    return (output);
}


/* ft_dirent_comparea - compare in ascending order (alphanum) */
static inline int ft_dirent_comparea(struct dirent *d0, struct dirent *d1) {
    const char *n0 = d0->d_name;
    const char *n1 = d1->d_name;
    
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


/* ft_dirent_compared - compare in descending order (alphanum) */
static inline int ft_dirent_compared(struct dirent *d0, struct dirent *d1) {
    const char *n0 = d0->d_name;
    const char *n1 = d1->d_name;

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


/* ft_dirent_compareat - compare in ascending order (time) */
static inline int ft_dirent_compareat(struct dirent *d0, struct dirent *d1) {
    struct stat st = { 0 };
    
    if (stat(d0->d_name, &st) == -1) { return (0); }
    size_t t0 = st.st_mtime;
    
    if (stat(d1->d_name, &st) == -1) { return (0); }
    size_t t1 = st.st_mtime;

    return (t0 >= t1);
}


/* ft_dirent_comparedt - compare in descending order (time) */
static inline int ft_dirent_comparedt(struct dirent *d0, struct dirent *d1) {
    struct stat st = { 0 };
    
    if (stat(d0->d_name, &st) == -1) { return (0); }
    size_t t0 = st.st_mtime;
    
    if (stat(d1->d_name, &st) == -1) { return (0); }
    size_t t1 = st.st_mtime;

    return (t0 <= t1);
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
