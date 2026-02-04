#include "./ft_ls.h"


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
