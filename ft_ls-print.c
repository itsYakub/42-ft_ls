#include "./ft_ls.h"


static int ft_print_column(struct s_file *, const size_t);


static int ft_print_vertical(struct s_file *, const size_t);


static int ft_print_long(struct s_file *, const size_t);


extern int ft_print(struct s_file *arr, const size_t size) {
    if (!arr) { return (0); }

    int status = 0;
    switch (g_print_mode) {
        case (PRINT_MODE_COLUMN):   { status = ft_print_column(arr, size);   } break;
        case (PRINT_MODE_VERTICAL): { status = ft_print_vertical(arr, size); } break;
        case (PRINT_MODE_LONG):     { status = ft_print_long(arr, size);     } break;

        default: { /* ... */ } break;
    }
    
    return (status);
}

static int ft_print_column(struct s_file *arr, const size_t size) {
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


/* SOURCE: https://stackoverflow.com/a/75575528
 * */
static int ft_print_vertical(struct s_file *arr, const size_t size) {
    if (!arr) { return (0); }

    /* get terminal resolution... */
    size_t width  = 0,
           height = 0;
    ft_getwinsize(&width, &height);
   
    /* ... */
    (void) arr;
    (void) size;

    return (1);
}


static int ft_print_long(struct s_file *arr, const size_t size) {
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
