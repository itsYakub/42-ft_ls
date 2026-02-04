#include "./ft_ls.h"

/* SOURCE: https://github.com/coreutils/coreutils/blob/master/src/ls.c#L175
 * */
static char g_type_str[] = {
    '?',    /* DT_UNKNOWN */
    'p',    /* DT_FIFO */
    'c',    /* DT_CHR */
     0,
    'd',    /* DT_DIR */
     0,
    'b',    /* DT_BLK */
     0,
    '-',    /* DT_REG */
     0,
    'l',    /* DT_LNK */
     0,
    's',    /* DT_SOCK */
     0,
    'w',    /* DT_WHT */
};


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


struct s_entry {
    struct passwd *passwd;
    struct group *group; 
    char *nlink;
    char *fsize;
    char *ctime;
    char *dname;
    char perm[11];

    int valid;
};

extern char *ft_print_list(struct dirent **dirents) {
    if (!dirents) { return (0); }

    /* pre-allocate s_entry array... */
    size_t dircnt = 0;
    while (dirents[dircnt]) {
        dircnt++;
    }

    struct s_entry *entries = calloc(dircnt, sizeof(struct s_entry));
    if (!entries) {
        return (0);
    }
    
    size_t blktotal = 0;
    for (size_t i = 0; dirents[i]; i++) {
        struct dirent *dirent = dirents[i];
        /* validate '-a' flag... */
        if (*dirent->d_name == '.') {
            if (!g_opt_all) {
                continue;
            }
        }

        struct stat st = { 0 };
        if (stat(dirent->d_name, &st) == -1) {
            continue;
        }
        blktotal += st.st_blocks / 2;

        struct s_entry entry = { 0 };

        /* file type char... */
        entry.perm[0] = g_type_str[dirent->d_type];
        /* user/owner... */
        entry.perm[1] = (st.st_mode & S_IRUSR) ? 'r' : '-';
        entry.perm[2] = (st.st_mode & S_IWUSR) ? 'w' : '-';
        entry.perm[3] = (st.st_mode & S_IXUSR) ? 'x' : '-';
        /* group... */
        entry.perm[4] = (st.st_mode & S_IRGRP) ? 'r' : '-';
        entry.perm[5] = (st.st_mode & S_IWGRP) ? 'w' : '-';
        entry.perm[6] = (st.st_mode & S_IXGRP) ? 'x' : '-';
        /* other... */
        entry.perm[7] = (st.st_mode & S_IROTH) ? 'r' : '-';
        entry.perm[8] = (st.st_mode & S_IWOTH) ? 'w' : '-';
        entry.perm[9] = (st.st_mode & S_IXOTH) ? 'x' : '-';

        /* file's number of links... */
        entry.nlink = ft_utoa(st.st_nlink);

        /* file's user... */
        entry.passwd = getpwuid(st.st_uid);
        
        /* file's group... */
        entry.group = getgrgid(st.st_gid);

        /* file size... */
        entry.fsize = ft_utoa(st.st_size);
        if (!*entry.fsize) {
            free(entry.fsize);
            entry.fsize = ft_strdup("0");
        }
        
        /* file size... */
        entry.ctime = ft_strdup(ctime(&st.st_mtime));
        *ft_strrchr(entry.ctime, '\n') = 0;
        
        /* file name... */
        entry.dname = dirent->d_name;

        entry.valid = 1;

        entries[i] = entry;
    }

    /* get the longest file size...*/
    size_t longest_size = 0;
    for (size_t i = 0; i < dircnt; i++) {
        if (!entries[i].valid) {
            continue;
        }
        size_t slen = ft_strlen(entries[i].fsize);
        longest_size = longest_size < slen ?
            slen : longest_size;
    }

    /* prepend total blok size to output... */
    char *tmp0 = ft_utoa(blktotal);
    char *output = ft_strjoin("total ", tmp0);
    output = ft_strjoin_free(output, "\n");
    free(tmp0), tmp0 = 0;

    for (size_t i = 0; i < dircnt; i++) {
        if (!entries[i].valid) {
            continue;
        }

        /* append perm... */
        output = ft_strjoin_free(output, entries[i].perm);
        output = ft_strjoin_free(output, " ");
        
        /* append number of links... */
        output = ft_strjoin_free(output, entries[i].nlink);
        output = ft_strjoin_free(output, " ");
        free(entries[i].nlink);
        
        /* append user name... */
        output = ft_strjoin_free(output, entries[i].passwd->pw_name);
        output = ft_strjoin_free(output, " ");
        
        /* append group name... */
        output = ft_strjoin_free(output, entries[i].group->gr_name);
        output = ft_strjoin_free(output, " ");
        
        /* append file size... */
        size_t slen = ft_strlen(entries[i].fsize);
        for (size_t i = 0; i < longest_size - slen; i++) {
            output = ft_strjoin_free(output, " ");
        }
        output = ft_strjoin_free(output, entries[i].fsize);
        output = ft_strjoin_free(output, " ");
        free(entries[i].fsize);
        
        /* append ctime... */
        output = ft_strjoin_free(output, entries[i].ctime);
        output = ft_strjoin_free(output, " ");
        free(entries[i].ctime);
        
        /* append file name... */
        output = ft_strjoin_free(output, entries[i].dname);
        output = ft_strjoin_free(output, "\n");
    }
    free(entries);
    return (output);
}
