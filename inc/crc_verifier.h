#ifndef __CRC_VERIFIER_H__
#define __CRC_VERIFIER_H__

#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "hashtable.h"

#define VERIFICATION_SUCCESS 0
#define VERIFICATION_FAILED -42

#define DB_ENTRY_DELIM ' '
#define FNAMESIZ 256 + 1
#define CRCVALSIZ 8 + 1
#define ENTRYSIZ FNAMESIZ + CRCVALSIZ

typedef unsigned char uchar;

// Util functions
void crc32b(uint32_t *crc, uchar *data, long nbytes);
uint32_t calc_file_crc(FILE *fp);
int verify(char *fname);
int is_a_dot(char *fname);
int traverse(char *dname);

void parse_entry(char *entry, char *fname, char *crc);

uint32_t verified_cnt;
struct hashtable htable;

#endif