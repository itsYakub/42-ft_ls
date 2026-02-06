#if !defined (_ft_ls_h_)
# define _ft_ls_h_ 1
#
# include <stdio.h>      /* perror() */
# include <stdlib.h>     /* malloc(), free(), exit() */
#
# include <time.h>       /* time(), ctime() */
# include <errno.h>
# include <string.h>     /* strerror() */
#
# include <pwd.h>        /* getpwuid() */
# include <grp.h>        /* getgrgid() */
# include <dirent.h>     /* opendir(), readdir(), closedir() */
# include <unistd.h>     /* write(), readlink() */
#
# include <sys/stat.h>   /* stat(), lstat()*/
# include <sys/types.h>
# include <sys/xattr.h>  /* listxattr(), getxattr() */
#
# include <linux/limits.h>
#
# include "./libft/libft.h"

extern int g_opt_list;

extern int g_opt_recursive;

extern int g_opt_all;

extern int g_opt_reverse;

extern int g_opt_time;

extern t_list *g_paths;

extern const char *g_prog;

/* ./ft_ls.c */

extern char *ft_process_subdirs(const char *, t_list *);

/* ./ft_ls-utils.c */

extern int ft_strcmp(const char *, const char *);


extern int ft_strendswith(const char *, int);


extern char *ft_strjoin_free(char *, const char *);


extern size_t ft_dircnt(const char *);


extern size_t ft_dirent_blkcnt(struct dirent **);


extern size_t ft_dirent_fsizmax(struct dirent **);


extern size_t ft_numlen(long long, int);


extern char *ft_utoa(unsigned long long, char [16]);

/* ./ft_ls-getopt.c */

extern int ft_getopt(int, char **);

/* ./ft_ls-print.c */

extern char *ft_print_vertical(struct dirent **);


extern char *ft_print_list(struct dirent **);


extern char *ft_print_entry(struct dirent *);

/* ./ft_ls-sort.c */

extern struct dirent **ft_dirent_qsort(struct dirent **, const size_t, const size_t, int (*)(struct dirent *, struct dirent *));

#endif /* _ft_ls_h_ */
