#include "./ft_ls.h"

extern char *ft_print_vertical(struct dirent **dirents) {
    if (!dirents) { return (0); }

    char *output = 0;
    for (size_t i = 0; dirents[i]; i++) {
        struct dirent *dirent = dirents[i];
        /* validate '-a' flag... */
        if (*dirent->d_name == '.') {
            if (!g_opt_all) {
                continue;
            }
        }

        output = output == 0 ?
            ft_strdup(dirent->d_name) :
            ft_strjoin_free(output, dirent->d_name);
        output = ft_strjoin_free(output, "\n");
    }
    return (output);
}


extern char *ft_print_list(struct dirent **dirents) {
    if (!dirents) { return (0); }

    size_t d_blkcnt   = ft_dirent_blkcnt(dirents) / 2;
    char d_blkstr[16] = { 0 };

    /* prepend total blok size to output... */
    char *output = ft_strjoin("total ", ft_utoa(d_blkcnt, d_blkstr));
    output = ft_strjoin_free(output, "\n");
    return (output);
}


extern char *ft_print_entry(struct dirent *dirent) {
    if (!dirent) { return (0); }

    (void) dirent;
    return (0);
}
