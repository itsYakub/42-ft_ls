#include "./ft_ls.h"


extern int ft_strcmp(const char *s0, const char *s1) {
    while (*s0 || *s1) {
        if (*s0 != *s1) { return (*s0 - *s1); }

        s0++;
        s1++;
    }
    return (0);
}


extern int ft_strendswith(const char *s0, int c) {
    if (!s0) { return (0); }
    while (*s0) { s0++; }

    return (*(s0 - 1) == c);
}


extern char *ft_strjoin_free(char *dst, const char *src) {
    char *tmp = ft_strjoin(dst, src);
    if (!tmp) {
        free(dst); return (0);
    }

    free(dst), dst = tmp;
    return (dst);
}


extern size_t ft_dircnt(const char *path) {
    DIR *dir = opendir(path);
    if (!dir) { return (0); }

    struct dirent *dirent = readdir(dir);
    if (!dirent) {
        closedir(dir); return (0);
    }

    size_t i = 0;
    while (dirent) {
        i++, dirent = readdir(dir);
    }

    closedir(dir);
    return (i);
}
