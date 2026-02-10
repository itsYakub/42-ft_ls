#include "./ft_ls.h"

/*  ft_ls:
 *  - [ ] leaks
 *  - [X] error messages
 * */

static int ft_extract_from_av(t_list **, t_list **, int, char **);


int main(int ac, char **av) {
    /* process command-line arguments... */

    int getopt = ft_getopt(ac, av);
    if (getopt != 0) {
        return (1);
    }

    /* extract files / directories... */
   
    t_list *l_file = 0;
    t_list *l_dirs  = 0;
    if (!ft_extract_from_av(&l_file, &l_dirs, ac, av)) {
        return (1);
    }

    /* process files... */

    if (l_file) {
        struct s_file *arr = ft_process_f(l_file);
        if (!arr) {
            ft_lstclear(&l_dirs, free);
            return (1);
        }

        size_t size = 0;
        while (*arr[size].f_name) { size++; }

        ft_print(arr, size, FILE_MODE_F);
        if (l_dirs) {
            ft_putchar_fd('\n', 1);
        }
        free(arr), arr = 0;
    }

    /* process directories... */
   
    if (l_dirs) {
        l_dirs = ft_lstsort(l_dirs);

        for (t_list *list = l_dirs; list; list = list->next) {
            const char *path = list->content;
            size_t size = ft_dircnt(path);
            if (!size) {
                ft_putstr_fd(g_prog, 2);
                ft_putstr_fd(": cannot open directory '", 2);
                ft_putstr_fd(path, 2);
                ft_putstr_fd("': ", 2);
                ft_putendl_fd(strerror(errno), 2);
                continue;
            }
            
            struct s_file *arr= ft_process_d(list);
            if (!arr) { 
                ft_lstclear(&l_file, free);
                ft_lstclear(&l_dirs, free);
                return (1);
            }

            size_t lstsize = ft_lstsize(l_dirs);
            if (lstsize > 1 || l_file) {
                if (list != l_dirs) {
                    ft_putchar_fd('\n', 1);
                }
                ft_putstr_fd(path, 1);
                ft_putendl_fd(":", 1);
            }

            if (size < 2) {
                if (g_opt_all) {
                    ft_print(arr, size, FILE_MODE_D);
                }
            }
            else {
                ft_print(arr, size, FILE_MODE_D);
            }

            free(arr), arr = 0;
        }
    }
        
    ft_lstclear(&l_file, free);
    ft_lstclear(&l_dirs, free);

    return (0);
}


static int ft_extract_from_av(t_list **l_file, t_list **l_dirs, int ac, char ** av) {
    if (!l_file) { return (0); }
    if (!l_dirs) { return (0); }

    for (size_t i = 1; i < (size_t) ac; i++) {
        if (*av[i] == '-') { continue; }

        struct stat st = { 0 };
        if (stat(av[i], &st) == -1) {
            ft_putstr_fd(g_prog, 2);
            ft_putstr_fd(": cannot access '", 2);
            ft_putstr_fd(av[i], 2);
            ft_putstr_fd("': ", 2);
            ft_putendl_fd(strerror(errno), 2);
            continue;
        }

        char *path = ft_strdup(av[i]);
        switch ((st.st_mode & S_IFMT)) {
            case (S_IFDIR): { ft_lstadd_back(&(*l_dirs), ft_lstnew(path)); } break;
            default: {
                ft_lstadd_back(&(*l_file), ft_lstnew(path));
            }
        }
    }
    
    /* default case... */
    if (!*l_file && !*l_dirs) {
        *l_dirs = ft_lstnew(ft_strdup("."));
    }

    return (1);
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
