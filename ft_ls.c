#include "./ft_ls.h"

/*  ft_ls:
 *  - required flags:
 *      > [ ] -l: use a long listing format
 *      > [ ] -R: list subdirectories recursively
 *      > [ ] -a: do not ignore entries starting with .
 *      > [ ] -r: reverse order while sorting
 *      > [ ] -t: sort by time, newest first; see --time (time/date format with -l; see TIME_STYLE below)
 * */

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

    /* 1. allocate an array for all directory entries... */
    size_t dirndx = 0;
    size_t dircnt = ft_dircnt(path);
    struct dirent **dirents = calloc(dircnt + 1, sizeof(struct dirent *));
    if (!dirents) {
        return (0);
    }

        /* 2. open directory on path 'path->content'... */
        DIR *dir = opendir(path);
        if (!dir) {
            free(dirents); return (0);
        }

            /* 3. extract every directory entry to dirents... */
            for (struct dirent *dirent = readdir(dir); dirent; dirent = readdir(dir)) {
                dirents[dirndx++] = dirent;
            }

                /* 4. process directory entries... */
                char *output = 0;

                /* 4.1. insert every name to output string */
                for (size_t i = 0; i < dircnt; i++) {
                    struct dirent dirent = *dirents[i];
                    
                    output = output == 0 ?
                        ft_strdup(dirent.d_name) :
                        ft_strjoin_free(output, dirent.d_name);

                    output = ft_strjoin_free(output, "\n");
                }

                /* 4.2. process subdirectories recursively... */
                for (size_t i = 0; i < dircnt; i++) {
                    struct dirent dirent = *dirents[i];

                    /* avoid "cwd" and "pwd" dirents... */
                    if (!ft_strcmp(dirent.d_name, ".") ||
                        !ft_strcmp(dirent.d_name, "..")
                    ) {
                        continue;
                    }

                    if (dirent.d_type == DT_DIR) {
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
                            free(subpath), subpath = 0;
                            continue;
                        }

                        output = ft_strjoin_free(output, "\n");
                        output = ft_strjoin_free(output, subpath);
                        output = ft_strjoin_free(output, ":\n");
                        output = ft_strjoin_free(output, tmp0);
                        
                        free(subpath), subpath = 0;
                        free(tmp0), tmp0 = 0;
                    }
                }

    /* 5. cleanup... */
    closedir(dir);
    free(dirents);

    return (output);
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
