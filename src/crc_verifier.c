#include "crc_verifier.h"

extern struct hashtable htable;

int main(int argc, char *argv[])
{
    int rc = VERIFICATION_SUCCESS;
    char ch;
    double elapsed;
    char db_raw_entry[ENTRYSIZ];
    char db_fname[FNAMESIZ];
    char db_crc[CRCVALSIZ];
    uint32_t crc, lines_cnt = 0;
    clock_t t;
    FILE *db_fp;

    if (argc < 3)
    {
        puts("Usage: ./crc_verifier ROOT_DIR CRC_DATABASE_FILE");
        return -1;
    }

    // Open 'CRC_DATABASE_FILE'
    db_fp = fopen(argv[2], "r");
    if (db_fp == NULL)
    {
        puts("Can't open database");
        return -2;
    }

    // Calculate the number of files to verify
    while ((ch = fgetc(db_fp)) != EOF)
    {
        if (ch == '\n') lines_cnt++;
    }

    rewind(db_fp);

    hashtable_init(&htable, lines_cnt);

    // Fill hashtable with 'CRC_DATABASE_FILE' content
    while (fgets(db_raw_entry, ENTRYSIZ, db_fp) != NULL)
    {
        parse_entry(db_raw_entry, db_fname, db_crc);
        crc = (uint32_t)strtoul(db_crc, NULL, 16);
        hashtable_add(&htable, db_fname, crc);
    }

    fclose(db_fp);

    hashtable_print(&htable);

    puts("Start verification");
    verified_cnt = 0;

    // Change process CWD to 'ROOT_DIR', it's necessary for
    // correct comparison with entries from 'CRC_DATABASE_FILE'
    rc = chdir(argv[1]);
    if (rc < 0) {
        puts("Can't change dir to 'ROOT_DIR'");
        return -3;
    }

    t = clock();
    rc = traverse("."); // traverse thru all files in a 'ROOT_DIR'
    t = clock() - t;
    elapsed = ((double) t) / CLOCKS_PER_SEC;

    printf((rc == VERIFICATION_SUCCESS) ? "Success: " : "Failed: ");
    printf("%u of %u files verified\n", verified_cnt, lines_cnt);
    printf("Elapsed: %lf sec\n", elapsed);

    hashtable_free(&htable);
    return rc;
}

void parse_entry(char *entry, char *fname, char *crc)
{
    uint32_t i, j;

    for (i = 0; entry[i] != DB_ENTRY_DELIM; i++)
        fname[i] = entry[i];
    fname[i] = '\0';
    i += 1;

    for (j = 0; entry[i + j] != '\n' && j < CRCVALSIZ - 1; j++)
        crc[j] = entry[i + j];
    crc[j] = '\0';
}