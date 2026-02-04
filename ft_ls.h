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

/* ./ft_ls-utils.c */

extern int ft_strcmp(const char *, const char *);


extern char *ft_strjoin_free(char *, const char *);

/* ./ft_ls-getopt.c */

extern int ft_getopt(int, char **);

#endif /* _ft_ls_h_ */
