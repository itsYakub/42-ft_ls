#include "./ft_ls.h"


extern int ft_strcmp(const char *s0, const char *s1) {
    while (*s0 || *s1) {
        if (*s0 != *s1) { return (*s0 - *s1); }

        s0++;
        s1++;
    }
    return (0);
}


extern int ft_strlast(const char *s0, int c) {
    if (!s0) { return (0); }
    while (*s0) { s0++; }

    return (*(s0 - 1) == c);
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


extern char *ft_utoa(uint64_t n, uint8_t radix, char buffer[16]) {
    const char base[] = "0123456789abcdef";

    /* length of the number... */
    size_t n_l = 0;
    for (uint64_t tmp = n; tmp != 0; tmp /= radix) {
        n_l++;
    }

    /* append characters to array... */
    for (size_t i = 0; n; i++, n /= 16) {
        buffer[i] = base[n % radix];
    }

    /* reverse the array... */
    for (size_t i = 0; i < n_l / 2; i++) {
        char tmp = buffer[i];
        buffer[i] = buffer[n_l - 1 - i];
        buffer[n_l - 1 - i] = tmp;
    }

    return (buffer);
}


extern int ft_getwinsize(size_t *w_ptr, size_t *h_ptr) {
    int fd = ft_open("/dev/tty", O_RDONLY, 0);
    if (fd == -1) {
        fd = STDOUT_FILENO;
    }

    struct winsize winsize = { 0 };
    if (ft_ioctl(fd, TIOCGWINSZ, &winsize) == -1) {
        if (w_ptr) { *w_ptr = 0; }
        if (h_ptr) { *h_ptr = 0; }
        return (0);
    }

    if (w_ptr) { *w_ptr = winsize.ws_col; }
    if (h_ptr) { *h_ptr = winsize.ws_row; }

    if (fd != STDOUT_FILENO) {
        ft_close(fd);
    }
    return (1);
}
