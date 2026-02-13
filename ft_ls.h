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
# include <fcntl.h>
# include <dirent.h>     /* opendir(), readdir(), closedir() */
# include <unistd.h>     /* write(), readlink() */
#
# include <sys/stat.h>   /* stat(), lstat()*/
# include <sys/types.h>
# include <sys/ioctl.h>
# include <sys/xattr.h>  /* listxattr(), getxattr() */
#
# include <linux/limits.h>
#
# include "./libft/libft.h"

extern int g_opt_recursive;

extern int g_opt_all;

extern int g_opt_reverse;

extern int g_opt_time;

extern size_t g_width;

extern size_t g_height;

extern char *g_prog;


enum e_print_mode {

    PRINT_MODE_COLUMN   = 0,    /* $ ls --format=single-column */
    PRINT_MODE_VERTICAL = 1,    /* $ ls --format=vertical */
    PRINT_MODE_LONG     = 2,    /* $ ls --format=long */

    /* ... */

    PRINT_MODE_COUNT

};

extern enum e_print_mode g_print_mode;


enum e_file_mode {
    FILE_MODE_F = 0,
    FILE_MODE_D = 1,

    /* ... */

    FILE_MODE_COUNT
};


struct s_file {
    char f_name[PATH_MAX];

    off_t f_size;

    mode_t f_mode;

    time_t f_mtime;
    struct timespec f_mtim;

    time_t f_ctime;
    struct timespec f_ctim;

    time_t f_atime;
    struct timespec f_atim;

    uid_t f_uid;
    gid_t f_gid;

    nlink_t f_nlink;

    blkcnt_t f_blkcnt;

    dev_t f_dev;
    dev_t f_rdev;

    char f_valid : 1;
};


/* ./ft_ls-dir.c */

extern struct s_file *ft_process_d(t_list *);

/* ./ft_ls-file.c */

extern struct s_file *ft_process_f(t_list *);


extern int ft_comparefa(void *, void *);


extern int ft_comparefd(void *, void *);


extern int ft_comparefat(void *, void *);


extern int ft_comparefdt(void *, void *);

/* ./ft_ls-utils.c */

extern int ft_strcmp(const char *, const char *);


extern int ft_strlast(const char *, int);


extern size_t ft_dircnt(const char *);


extern size_t ft_numlen(const size_t, const uint8_t);


extern char *ft_utoa(uint64_t, uint8_t, char[16]);


extern int ft_getwinsize(size_t *, size_t *);


extern int ft_swap(void *, void *, size_t);

/* ./ft_ls-getopt.c */

extern int ft_getopt(int, char **);

/* ./ft_ls-print.c */

extern int ft_print(struct s_file *, const size_t, int);


/* ./ft_ls-lstsort.c */

extern int ft_comparela(void *, void *);


extern int ft_compareld(void *, void *);


extern int ft_comparelat(void *, void *);


extern int ft_compareldt(void *, void *);


extern t_list *ft_lstsort(t_list *);

/* ./ft_ls-sort.c */

extern void *ft_qsort(void *, const size_t, const size_t, const size_t, int (*)(void *, void *));

#endif /* _ft_ls_h_ */
