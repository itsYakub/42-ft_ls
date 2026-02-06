#include "./ft_ls.h"

extern int ft_print_column(struct s_file *arr, const size_t size) {
    if (!arr) { return (0); }

    for (size_t i = 0; i < size; i++) {
        /* validate '-a' flag... */
        if (*arr[i].f_name == '.') {
            if (!g_opt_all) {
                continue;
            }
        }

        ft_putendl_fd(arr[i].f_name, 1);
    }
    return (1);
}


extern int ft_print_vertical(struct s_file *arr, const size_t size) {
    if (!arr) { return (0); }

    (void) arr;
    (void) size;
    return (1);
}


extern int ft_print_long(struct s_file *arr, const size_t size) {
    if (!arr) { return (0); }

    (void) arr;
    (void) size;
    return (1);
}
