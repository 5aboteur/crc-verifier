#include "crc_verifier.h"

extern uint32_t verified_cnt;
extern struct hashtable htable;

void crc32b(uint32_t *crc, uchar *data, long nbytes)
{
    int i, j;
    uint32_t byte, mask;

    for (i = 0; i < nbytes; i++)
    {
        byte = data[i];
        *crc = *crc ^ byte;

        for (j = 7; j >= 0; j--)
        {
            mask = -(*crc & 1);
            *crc = (*crc >> 1) ^ (0xEDB88320 & mask);
        }
    }
}

uint32_t calc_file_crc(FILE *fp)
{
    long nbytes;
    uint32_t crc;
    uchar *buf = malloc(sizeof(uchar) * BUFSIZ);

    crc = 0xFFFFFFFF;

    while ((nbytes = fread(buf, sizeof(uchar), BUFSIZ, fp)) > 0)
    {
        crc32b(&crc, buf, nbytes);
    }

    free(buf);
    return ~crc;
}

int verify(char *fname)
{
    int rc;
    uint32_t db_crc, file_crc;
    FILE *fp;

    fp = fopen(fname, "rb");
    if (fp == NULL)
    {
        fprintf(stderr, "\nUnable to open: %s\n", fname);
        return -2;
    }

    file_crc = calc_file_crc(fp);
    db_crc = hashtable_get(&htable, fname);

    if (file_crc == db_crc)
    {
        rc = VERIFICATION_SUCCESS;
        verified_cnt++;
        hashtable_checkmark(&htable, fname);
        update_progress_bar((double)verified_cnt / htable.nelems);
    }
    else
    {
        fprintf(stderr, "\nStopped at '%s' (%x != %x)\n",
            fname, file_crc, db_crc);
        rc = VERIFICATION_FAILED;
    }

    fclose(fp);
    return rc;
}

void update_progress_bar(double pctg)
{
    int val = (int)(pctg * 100);
    int lpad = (int)(pctg * PB_WIDTH);
    int rpad = PB_WIDTH - lpad;
    printf("\r%4d%% [%.*s%*s]", val, lpad, PB_STR, rpad, "");
    fflush(stdout);
}

int is_a_dot(char *fname)
{
    return (!strcmp(fname, ".") || !strcmp(fname, ".."));
}

int traverse(char *dname)
{
    int rc = 0;
    char fname[FNAMESIZ];
    struct dirent *dentry;
    struct stat stbuf;
    DIR *dir;

    dir = opendir(dname);
    if (dir == NULL)
    {
        printf("\nCan't open directory: %s\n", dname);
        return -5;
    }

    while ((dentry = readdir(dir)) != NULL)
    {
        sprintf(fname, "%s/%s", dname, dentry->d_name); // full path
        if ((lstat(fname, &stbuf)) == -1)
        {
            printf("\nUnable to stat file: %s\n", fname);
            rc = -6;
            break;
        }

        if (S_ISDIR(stbuf.st_mode))
        {
            if (!is_a_dot(dentry->d_name))
            {
                rc = traverse(fname);
                if (rc != 0) break;
            }
        }
        else if (S_ISREG(stbuf.st_mode)
            && (stbuf.st_mode & S_IXUSR)
            && (!S_ISLNK(stbuf.st_mode)))
        {
            rc = verify(fname);
            if (rc != VERIFICATION_SUCCESS) break;
        }
        else { /* skip non-executables */ }
    }

    closedir(dir);
    return rc;
}
