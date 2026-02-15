#include "./ft_ls.h"


static int ft_extract(struct s_file *, const char *, const char *);


static struct s_file *ft_sort(struct s_file *, const size_t);


static int ft_recurse(struct s_file *, const size_t, const char *, t_list **);


extern struct s_file *ft_process_d(t_list *list, size_t *size) {
    if (!list) { return (0); }

    const char *path = list->content;
    DIR *dir = opendir(path);
    if (!dir) {
        return (0);
    }

    *size = ft_dircnt(path);
    if (!*size) { return (0); }

    struct s_file *arr = ft_calloc(*size, sizeof(struct s_file));
    if (!arr) { return (0); }

    struct dirent *dirent = 0;
    for (size_t i = 0; (dirent = readdir(dir)); i++) {        
        if (ft_extract(&arr[i], path, dirent->d_name)) { }
    }
    closedir(dir);
  
    arr = ft_sort(arr, *size);
    if (!arr) {
        return (0);
    }

    if (g_opt_recurse) {
        ft_recurse(arr, *size, path, &list);
    }
    
    return (arr);
}


static int ft_extract(struct s_file *result, const char *path, const char *name) {
    if (!path)   { return (0); }
    if (!result) { return (0); }
    
    char subpath[PATH_MAX] = { 0 };
    ft_create_subpath(path, name, subpath);
    
    struct stat st = { 0 };
    if (lstat(subpath, &st) == -1) {
        return (0);
    }
    ft_strlcat(result->f_name, name, PATH_MAX);
    ft_strlcat(result->f_path, path, PATH_MAX);
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

    int (*compare)(void *, void *) = 0;
    if (g_opt_reverse) {
        if (g_opt_time) {
            compare = ft_comparefdt;
        }
        else {
            compare = ft_comparefd;
        }
    }
    else if (g_opt_time) {
        compare = ft_comparefat;
    }
    else {
        compare = ft_comparefa;
    }

    return (ft_qsort(arr, sizeof(*arr), 0, size - 1, compare));
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
            ft_create_subpath(path, arr[i].f_name, subpath);

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
