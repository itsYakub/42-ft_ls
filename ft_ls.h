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

extern int g_opt_recursive;

extern int g_opt_all;

extern int g_opt_reverse;

extern int g_opt_time;

extern char *g_prog;


enum e_print_mode {

    PRINT_MODE_COLUMN   = 0,    /* $ ls --format=single-column */
    PRINT_MODE_VERTICAL = 1,    /* $ ls --format=vertical */
    PRINT_MODE_LONG     = 2,    /* $ ls --format=long */

    /* ... */

    PRINT_MODE_COUNT

};

extern enum e_print_mode g_print_mode;


struct s_file {
    char f_name[PATH_MAX];

    mode_t f_mode;

    time_t f_mtime;
    time_t f_ctime;
    time_t f_atime;

    uid_t f_uid;
    gid_t f_gid;

    nlink_t f_nlink;

    blkcnt_t f_blkcnt;

    dev_t f_dev;
    dev_t f_rdev;
};


/* ./ft_ls-dir.c */

extern struct s_file *ft_process_d(t_list *);

/* ./ft_ls-file.c */

extern struct s_file *ft_process_f(t_list *);

/* ./ft_ls-utils.c */

extern int ft_strcmp(const char *, const char *);


extern int ft_strendswith(const char *, int);


extern char *ft_strjoin_free(char *, const char *);


extern size_t ft_dircnt(const char *);


extern size_t ft_numlen(long long, int);


extern char *ft_utoa(unsigned long long, char [16]);

/* ./ft_ls-getopt.c */

extern int ft_getopt(int, char **);

/* ./ft_ls-print.c */

extern int ft_print(struct s_file *, const size_t);


/* ./ft_ls-sort.c */

extern struct s_file *ft_qsort(struct s_file *, const size_t, const size_t, int (*)(struct s_file, struct s_file));

#endif /* _ft_ls_h_ */
