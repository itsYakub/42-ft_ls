#include "./ft_ls.h"


static int ft_print_column(struct s_file *, const size_t);


static int ft_print_vertical(struct s_file *, const size_t);


static int ft_print_long(struct s_file *, const size_t, int);


extern int ft_print(struct s_file *arr, const size_t size, int mode) {
    if (!arr) { return (0); }

    int status = 0;
    switch (g_print_mode) {
        case (PRINT_MODE_COLUMN):   { status = ft_print_column(arr, size);     } break;
        case (PRINT_MODE_VERTICAL): { status = ft_print_vertical(arr, size);   } break;
        case (PRINT_MODE_LONG):     { status = ft_print_long(arr, size, mode); } break;

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
    int max_len[256];
};

static size_t ft_column_count(struct s_file *, const size_t, struct s_col_info *);

/* SOURCE:
 * - https://stackoverflow.com/a/75575528
 * - https://mmzeynalli.dev/posts/reinvent/ls/part5/#3-tabular
 * */
static int ft_print_vertical(struct s_file *arr, const size_t size) {
    if (!arr) { return (0); }

    /* filter files... */
    struct s_file *arr_f = ft_calloc(size, sizeof(struct s_file));
    size_t size_f = 0;
    for (size_t i = 0; i < size; i++) {
        /* validate '-a' flag... */
        if (*arr[i].f_name == '.') {
            if (!g_opt_all) {
                continue;
            }
        }

        arr_f[size_f++] = arr[i];
    } 

    /* allocate column info array... */
    struct s_col_info col_info[256] = { [0 ... 255 ] = { 1, 0, { 0 } } };
    
    size_t ncols = ft_column_count(arr_f, size_f, col_info);
    size_t nrows = ncols > 0 ? 
        (size_f + ncols - 1) / ncols : 0;

    struct s_col_info info = col_info[ncols - 1];
    for (size_t i = 0; i < nrows; i++) {
        for (size_t j = 0; j < ncols; j++) {
            size_t arr_i = j * nrows + i;
            struct s_file file = arr_f[arr_i];

            ft_putstr_fd(file.f_name, 1);
            if (j < ncols - 1) {
                size_t strlen = ft_strlen(file.f_name);
                size_t width  = info.max_len[j];
                for (size_t k = 0; k < width - strlen + 2; k++) {
                    ft_putchar_fd(' ', 1);
                }
            }
        }
        ft_putchar_fd('\n', 1);
    }

    free(arr_f);
    return (1);
}


static size_t ft_column_count(struct s_file *arr, const size_t size, struct s_col_info *info) {
    if (!arr)  { return (0); }
    if (!info) { return (0); }

    /* get terminal resolution... */
    size_t width  = g_width,
           height = g_height;

    (void) height;
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
                info[info_i].line_len += strlen - info[info_i].max_len[col];
                info[info_i].max_len[col] = strlen;
            }

            if ((size_t) info[info_i].line_len + (2 * info_i) > width) {
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

static char *ft_print_perm(struct s_file, char [16]);


static char *ft_print_date(struct s_file, char [128]);


static int ft_print_long(struct s_file *arr, const size_t size, int mode) {
    if (!arr) { return (0); }

    /* Total blocks and longest address...  */
    size_t blocks = 0;
    off_t sizemax = 0;
    nlink_t nlinkmax = 0;
    for (size_t i = 0; i < size; i++) {
        struct s_file file = arr[i];
        /* validate '-a' flag... */
        if (*file.f_name == '.') {
            if (!g_opt_all) {
                continue;
            }
        }

        blocks += file.f_blkcnt / 2;
        sizemax = file.f_size > sizemax ? file.f_size : sizemax;
        nlinkmax = file.f_nlink > nlinkmax ? file.f_nlink : nlinkmax;
    }
    sizemax  = ft_numlen(sizemax, 10);
    nlinkmax = ft_numlen(nlinkmax, 10);

    /* Total blocks... */
    if (mode == FILE_MODE_D) {
        ft_putstr_fd("total ", 1);
        ft_putnbr_fd(blocks, 1);
        ft_putchar_fd('\n', 1);
    }

    for (size_t i = 0; i < size; i++) {
        struct s_file file = arr[i];
        /* validate '-a' flag... */
        if (*file.f_name == '.') {
            if (!g_opt_all) {
                continue;
            }
        }

        /* print permissions... */
        char buffer[128] = { 0 };
        ft_print_perm(file, buffer);
        ft_putstr_fd(buffer, 1); ft_putchar_fd(' ', 1);

        /* print number of links... */
        size_t numlen = ft_numlen(file.f_nlink, 10);
        for (size_t i = 0; i < nlinkmax - numlen; i++) {
            ft_putchar_fd(' ', 1);
        }
        ft_putnbr_fd(file.f_nlink, 1); ft_putchar_fd(' ', 1);

        /* print owner... */
        struct passwd *passwd = getpwuid(file.f_uid);
        ft_putstr_fd(passwd->pw_name, 1); ft_putchar_fd(' ', 1);
        
        /* print group... */
        struct group *group = getgrgid(file.f_gid);
        ft_putstr_fd(group->gr_name, 1); ft_putchar_fd(' ', 1);

        /* print address... */
        numlen = ft_numlen(file.f_size, 10);
        for (size_t i = 0; i < sizemax - numlen; i++) {
            ft_putchar_fd(' ', 1);
        }
        ft_putnbr_fd(file.f_size, 1);
        ft_putchar_fd(' ', 1);

        /* print date... */
        ft_print_date(file, buffer);
        ft_putstr_fd(buffer, 1); ft_putchar_fd(' ', 1);

        /* print name... */
        ft_putstr_fd(arr[i].f_name, 1);

        ft_putchar_fd('\n', 1);
    }
    return (1);
}


static char *ft_print_perm(struct s_file file, char buffer[16]) {
    /* file type... */
    switch (file.f_mode & S_IFMT) {
        case (S_IFDIR):  { *buffer = 'd'; } break;
        case (S_IFCHR):  { *buffer = 'c'; } break;
        case (S_IFBLK):  { *buffer = 'b'; } break;
        case (S_IFREG):  { *buffer = '-'; } break;
        case (S_IFIFO):  { *buffer = 'f'; } break;
        case (S_IFLNK):  { *buffer = 'l'; } break;
        case (S_IFSOCK): { *buffer = 's'; } break;
        default: {
            *buffer = '?';
        } break;
    }

    /* user permissions... */
    buffer[1] = file.f_mode & S_IRUSR ? 'r' : '-';
    buffer[2] = file.f_mode & S_IWUSR ? 'w' : '-';
    buffer[3] = file.f_mode & S_IXUSR ? 'x' : '-';
    /* group permissions... */
    buffer[4] = file.f_mode & S_IRGRP ? 'r' : '-';
    buffer[5] = file.f_mode & S_IWGRP ? 'w' : '-';
    buffer[6] = file.f_mode & S_IXGRP ? 'x' : '-';
    /* other permissions... */
    buffer[7] = file.f_mode & S_IROTH ? 'r' : '-';
    buffer[8] = file.f_mode & S_IWOTH ? 'w' : '-';
    buffer[9] = file.f_mode & S_IXOTH ? 'x' : '-';

    /* null-terminator... */
    for (size_t i = 10; i < 1; i++) { buffer[i] = 0; }
    return (buffer);
}


static char *ft_print_date(struct s_file file, char buffer[128]) {
    const char *ct = ctime(&file.f_mtime);
    if (!ct) {
        return (0);
    }

    time_t t_now = time(0);

    ft_memset(buffer, 0, 128);
    /* time less than 6 months... */
    if (t_now - file.f_mtime < 15778458) {
        ft_strlcpy(&buffer[0], ct + 4, 13);
    }
    /* time greater than 6 months... */
    else {
        ft_strlcpy(&buffer[0], ct + 4, 8);
        ft_strlcpy(&buffer[7], ct + 19, 6);
    }
    return (buffer);
}
