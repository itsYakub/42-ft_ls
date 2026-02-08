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

/* ------------------------------------------------------------------------- */

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

/* ------------------------------------------------------------------------- */

struct s_col_info {
    int line_valid;
    int line_len;
    int *max_len;
};

static size_t ft_column_count(struct s_file *, const size_t, struct s_col_info *);

/* SOURCE:
 * - https://stackoverflow.com/a/75575528
 * - https://mmzeynalli.dev/posts/reinvent/ls/part5/#3-tabular
 * */
static int ft_print_vertical(struct s_file *arr, const size_t size) {
    if (!arr) { return (0); }

    /* allocate column info array... */
    struct s_col_info col_info[256] = { [0 ... 255 ] = { 1, 0, 0 } };
    for (size_t i = 0; i < 256; i++) {
        col_info[i].max_len = ft_calloc(size, sizeof(int));
    }

    size_t ncols = ft_column_count(arr, size, col_info);
    size_t nrows = (size + ncols - 1) / ncols;

    for (size_t i = 0; i < nrows; i++) {
        for (size_t j = 0; j < ncols; j++) {
            size_t arr_i = j * nrows + i;

            ft_putstr_fd(arr[arr_i].f_name, 1);
            if (j < ncols - 1) {
                size_t strlen = ft_strlen(arr[arr_i].f_name);
                size_t width  = col_info[ncols-1].max_len[j];
                for (size_t k = 0; k < width - strlen + 2; k++) {
                    ft_putchar_fd(' ', 1);
                }
            }
        }
        ft_putchar_fd('\n', 1);
    }

    for (size_t i = 0; i < 256; i++) {
        free(col_info[i].max_len);
    }

    return (1);
}


static size_t ft_column_count(struct s_file *arr, const size_t size, struct s_col_info *info) {
    if (!arr)  { return (0); }
    if (!info) { return (0); }

    /* get terminal resolution... */
    size_t width  = 0,
           height = 0;
    ft_getwinsize(&width, &height);

    size_t max_columns_count = width / (1 + 2); /* (1 + 2) -> 1 char name + 2 spaces */ 
    size_t max_columns = max_columns_count < size ? max_columns_count : size;

    for (size_t file_i = 0; file_i < size; file_i++) {
        for (size_t info_i = 0; info_i < max_columns; info_i++) {
            if (!info[info_i].line_valid) {
                continue;
            }

            size_t rows = (size + info_i) / (info_i + 1);
            if (rows * (info_i + 1) - size > rows) {
                info[info_i].line_valid = 0;
                continue;
            }

            size_t col = file_i / rows;
            size_t strlen = ft_strlen(arr[file_i].f_name);
            if (strlen > (size_t) info[info_i].max_len[col]) {
                info[info_i].line_len += strlen - info[info_i].max_len[col] + 2;
                info[info_i].max_len[col] = strlen;
            }

            if ((size_t) info[info_i].line_len > width) {
                info[info_i].line_valid = 0;
            }
        }
    }

    ssize_t selected = max_columns - 1;
    while (selected >= 0) {
        if (!info[selected].line_valid || !info[selected].max_len[selected]) {
            selected--;
        }
        else {
            break;
        }
    }
    return (selected + 1);
}

/* ------------------------------------------------------------------------- */

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
