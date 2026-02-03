#include <stdio.h>      /* perror() */
#include <stdlib.h>     /* malloc(), free(), exit() */

#include <time.h>       /* time(), ctime() */
#include <errno.h>
#include <string.h>     /* strerror() */

#include <pwd.h>        /* getpwuid() */
#include <grp.h>        /* getgrgid() */
#include <dirent.h>     /* opendir(), readdir(), closedir() */
#include <unistd.h>     /* write(), readlink() */

#include <sys/stat.h>   /* stat(), lstat()*/
#include <sys/types.h>
#include <sys/xattr.h>  /* listxattr(), getxattr() */

#include <linux/limits.h>


#include "./libft/libft.h"

/*  ft_ls:
 *  - required flags:
 *      > [ ] -l: use a long listing format
 *      > [ ] -R: list subdirectories recursively
 *      > [ ] -a: do not ignore entries starting with .
 *      > [ ] -r: reverse order while sorting
 *      > [ ] -t: sort by time, newest first; see --time (time/date format with -l; see TIME_STYLE below)
 * */

extern int g_opt_list;

extern int g_opt_recursive;

extern int g_opt_all;

extern int g_opt_reverse;

extern int g_opt_time;

extern t_list *g_paths;

extern const char *g_prog;

/* ft_ls-utils.c */

extern int ft_strcmp(const char *, const char *);

extern char *ft_strjoin_free(char *, const char *);

/* ft_ls-process.c */

extern char *ft_process_subdirs(const t_list *, const size_t);

/* ft_ls-getopt.c */

static int ft_getopt_help(void);

static int ft_getopt_version(void);

extern int ft_getopt(int, char **);

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

extern int ft_strcmp(const char *s0, const char *s1) {
    while (*s0 && *s1) {
        if (*s0 != *s1) { return (*s0 - *s1); }

        s0++;
        s1++;
    }
    return (0);
}

extern char *ft_strjoin_free(char *dst, const char *src) {
    char *tmp = ft_strjoin(dst, src);
    if (!tmp) {
        return (0);
    }

    free(dst), dst = tmp;
    return (tmp);
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

extern int ft_getopt(int ac, char **av) {
    (void) ac;
    
    g_prog = (const char *) *av;
    while (*++av) {
        /* process options... */
        if (**av == '-') {
            const char *opt = *av + 1;

            /* long-option... */
            if (*opt == '-') {
                opt++;

                if (!ft_strcmp(opt, "all"))       { g_opt_all = 1;       }
                if (!ft_strcmp(opt, "reverse"))   { g_opt_reverse = 1;   }
                if (!ft_strcmp(opt, "recursive")) { g_opt_recursive = 1; }
                
                else if (!ft_strcmp(opt, "help")) {
                    ft_getopt_help();
                }
                
                else if (!ft_strcmp(opt, "version")) {
                    ft_getopt_version();
                }

                else {
                    ft_putstr_fd(g_prog, 1);
                    ft_putstr_fd(": unrecognized option '--", 1);
                    ft_putstr_fd(opt, 1);
                    ft_putendl_fd("'", 1);
                    
                    ft_putstr_fd("Try '", 1);
                    ft_putstr_fd(g_prog, 1);
                    ft_putendl_fd(" --help' for more information", 1);

                    return (2);
                }
            }
            /* short-option... */
            else {
                do {
                    switch (*opt) {
                        case ('a'): { g_opt_all = 1;       } break;
                        case ('l'): { g_opt_list = 1;      } break;
                        case ('t'): { g_opt_time = 1;      } break;
                        case ('r'): { g_opt_reverse = 1;   } break;
                        case ('R'): { g_opt_recursive = 1; } break;

                        default: {
                            ft_putstr_fd(g_prog, 1);
                            ft_putstr_fd(": invalid option -- '", 1);
                            ft_putchar_fd(*opt, 1);
                            ft_putendl_fd("'", 1);
                            
                            ft_putstr_fd("Try '", 1);
                            ft_putstr_fd(g_prog, 1);
                            ft_putendl_fd(" --help' for more information", 1);

                            return (2);
                        }
                    }
                } while (*++opt);
            }
        }

        /* process files... */
        else {
            t_list *path = ft_lstnew(ft_strdup(*av));
            if (!path) {
                return (1);
            }

            ft_lstadd_back(&g_paths, path);
        }
    }
    return (0);
}

static int ft_getopt_help(void) {
    /* print usage... */
    ft_putstr_fd("Usage: ", 1);
    ft_putstr_fd(g_prog, 1);
    ft_putendl_fd(" [OPTION]... [FILE]...", 1);

    /* print description... */
    ft_putendl_fd("List information about the FILEs (the current directory by default).", 1);
    
    /* print options... */
    ft_putendl_fd("Mandatory arguments to long options are mandatory for short options too.", 1);
    ft_putendl_fd("  -a, --all                  do not ignore entries starting with .", 1);
    ft_putendl_fd("  -l                         use a long listing format", 1);
    ft_putendl_fd("  -r, --reverse              reverse order while sorting", 1);
    ft_putendl_fd("  -R, --recursive            list subdirectories recursively", 1);
    ft_putendl_fd("  -t                         sort by time, newest first", 1);
    ft_putendl_fd("      --help        display this help and exit", 1);
    ft_putendl_fd("      --version     output version information and exit", 1);
    
    /* print exit code status... */
    ft_putendl_fd("Exit status:", 1);
    ft_putendl_fd(" 0  if OK,", 1);
    ft_putendl_fd(" 1  if minor problems (e.g., cannot access subdirectory),", 1);
    ft_putendl_fd(" 2  if serious trouble (e.g., cannot access command-line argument).", 1);

    /* bug reports... */
    ft_putendl_fd("Report bugs to: https://github.com/itsYakub/42-ft_ls/issues", 1);
    
    /* exit the program... */
    if (g_paths) {
        ft_lstclear(&g_paths, free), g_paths = 0;
    }
    
    exit(0);
}

static int ft_getopt_version(void) {
    ft_putendl_fd("ft_ls (42 Warsaw) 1.0.0.", 1);
    ft_putendl_fd("Project subject version: 3.1.", 1);
    ft_putendl_fd("Project available under the GNU Lesser General Public License v.2.1.", 1);
    ft_putendl_fd("Source code available under <https://github.com/itsYakub/42-ft_ls>.", 1);

    /* exit the program... */
    if (g_paths) {
        ft_lstclear(&g_paths, free), g_paths = 0;
    }
    
    exit(0);
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
