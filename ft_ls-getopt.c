#include "./ft_ls.h"


static int ft_getopt_help(void);


static int ft_getopt_version(void);


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
                        case ('1'): { g_print_mode = 0;    } break;
                        case ('C'): { g_print_mode = 1;    } break;
                        case ('l'): { g_print_mode = 2;    } break;
                        case ('a'): { g_opt_all = 1;       } break;
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
    ft_putendl_fd("  -C                         list entries by columns", 1);
    ft_putendl_fd("  -l                         use a long listing format", 1);
    ft_putendl_fd("  -r, --reverse              reverse order while sorting", 1);
    ft_putendl_fd("  -R, --recursive            list subdirectories recursively", 1);
    ft_putendl_fd("  -t                         sort by time, newest first", 1);
    ft_putendl_fd("  -1                         list one file per line", 1);
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
