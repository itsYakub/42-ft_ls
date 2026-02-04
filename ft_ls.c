#include "./ft_ls.h"

/*  ft_ls:
 *  - required flags:
 *      > [ ] -l: use a long listing format
 *      > [ ] -R: list subdirectories recursively
 *      > [ ] -a: do not ignore entries starting with .
 *      > [ ] -r: reverse order while sorting
 *      > [ ] -t: sort by time, newest first; see --time (time/date format with -l; see TIME_STYLE below)
 * */

extern char *ft_process_subdirs(const t_list *, const size_t);

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
   
    char *output = ft_process_subdirs(g_paths, 0);
    if (!output) {
        exitcode = 1; goto main_exit;
    }

    ft_putendl_fd(output, 1);
    free(output), output = 0;

main_exit:

    if (g_paths) {
        ft_lstclear(&g_paths, free), g_paths = 0;
    }

    return (exitcode);
}


extern char *ft_process_subdirs(const t_list *paths, const size_t depth) {
    if (!paths) { return (0); }

    /* NOTE:
     *  This could be helpful while implementing recursive listing
     *  Could be replaced with "previous working directory" string that we can prepend before dirent->d_name
     * */
    (void) depth;

    /* Standard GNU ls iterates over every supplied path to the program
     * Even if recursion is enabled this still goes from supplied path to supplied path
     *
     * By that logic we can simply go from file to file, store the output into string and display it when needed
     * */
    char *output = 0;
    for (t_list *path = (t_list *) paths; path; path = path->next) {
        ft_putendl_fd(path->content, 1);

        /* NOTE:
         *  THeoretically speaking it would be better to store the pointers to the "dirent"-s
         *  We could then perform easier sorting, etc. Also, recursion would still be valid.
         * */
        DIR *dir = opendir(path->content);
        if (!dir) { return (0); }
        for (struct dirent *dirent = readdir(dir); dirent; dirent = readdir(dir)) {
            output = ft_strjoin_free(output, dirent->d_name);
            output = ft_strjoin_free(output, "  ");
        }

        output = ft_strjoin_free(output, "\n");
        closedir(dir);
    }

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
