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

int main(int ac, char **av) {
    (void) ac;
    (void) av;
    return (0);
}
