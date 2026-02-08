#include "./ft_ls.h"

/*  ft_ls:
 *  - required flags:
 *      > [X] -l: use a long listing format
 *      > [X] -R: list subdirectories recursively
 *      > [X] -a: do not ignore entries starting with .
 *      > [X] -r: reverse order while sorting
 *      > [X] -t: sort by time, newest first; see --time (time/date format with -l; see TIME_STYLE below)
 *  - [X] stylized formatting for regular output (check how ls libft/ behave)"
 *      > columns
 *      > top-to-bottom
 *      > based on length on single line + length of tty line, probably
 *  - [ ] fix the time sorting
 *  - [ ] linked list sorting
 *  - [X] figure out the COLUMNS and LINES
 * */

int main(int ac, char **av) {
    /* process command-line arguments... */

    int getopt = ft_getopt(ac, av);
    if (getopt != 0) {
        return (1);
    }

    /* extract files / directories... */
    
    t_list *l_files = 0;
    t_list *l_dirs  = 0;
    for (size_t i = 1; i < (size_t) ac; i++) {
        if (*av[i] == '-') { continue; }

        struct stat st = { 0 };
        if (stat(av[i], &st) == -1) {
            return (1);
        }

        char *path = ft_strdup(av[i]);
        switch ((st.st_mode & S_IFMT)) {
            case (S_IFDIR): { ft_lstadd_back(&l_dirs, ft_lstnew(path)); } break;
            default: {
                ft_lstadd_back(&l_files, ft_lstnew(path));
            }
        }
    }
    
    /* default case... */
    if (!l_files && !l_dirs) {
        l_dirs = ft_lstnew(ft_strdup("."));
    }

    /* process files... */

    if (l_files) {
        struct s_file *arr = ft_process_f(l_files);
        if (!arr) {
            ft_lstclear(&l_dirs, free);
            return (1);
        }

        size_t size = 0;
        while (*arr[size].f_name) { size++; }

        ft_print(arr, size, FILE_MODE_F);
        free(arr), arr = 0;
        
        ft_lstclear(&l_files, free);
    }

    /* process directories... */

    for (t_list *list = l_dirs; list; list = list->next) {
        struct s_file *arr= ft_process_d(list);
        if (!arr) {
            ft_lstclear(&l_dirs, free);
            return (1);
        }

        size_t size = 0;
        while (ft_isprint(*arr[size].f_name)) { size++; }
        
        const char *path = list->content;
        size_t lstsize = ft_lstsize(l_dirs);
        if (lstsize > 1) {
            if (list != l_dirs) {
                ft_putchar_fd(10, 1);
            }

            ft_putstr_fd(path, 1);
            ft_putendl_fd(":", 1);
        }

        ft_print(arr, size, FILE_MODE_D);
        free(arr), arr = 0;
    }

    ft_lstclear(&l_dirs, free);

    return (0);
}

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

/* g_prog - name of the executable (av[0])
 * */
char *g_prog = 0;

/* g_print_mode - mode of printing, selected by either '-1', '-C' or '-l' flag
 * */
enum e_print_mode g_print_mode = PRINT_MODE_VERTICAL;
