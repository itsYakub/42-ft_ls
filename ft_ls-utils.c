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


extern size_t ft_numlen(long long n, int base) {
	int	result = 0;
	while (n != 0) {
		result++;
		n /= base;
	}

	return (result);
}


extern char *ft_utoa(unsigned long long n) {
    const char base[] = "0123456789";

    /* length of the number in base16... */
    size_t n_l = ft_numlen(n, 10);

    /* output string... */
    char *output = ft_calloc(n_l + 1, sizeof(char));
    if (!output) { return (0); }

    /* append characters to array... */
    for (size_t i = 0; n; i++, n /= 10) {
        output[i] = base[n % 10];
    }

    /* reverse the array... */
    for (size_t i = 0; i < n_l / 2; i++) {
        char tmp = output[i];
        output[i] = output[n_l - 1 - i];
        output[n_l - 1 - i] = tmp;
    }

    return (output);
}
