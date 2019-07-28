#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define HASH_MUL 31

struct listnode {
    char *fname;
    uint32_t crc;
    uint8_t checked;
    struct listnode *next;
};

struct hashtable {
    uint32_t height;
    uint32_t nelems;
    struct listnode **table;
};

uint32_t hashtable_hash(char *_str, uint32_t _height);
void hashtable_init(struct hashtable *_htable, uint32_t _height);
void hashtable_free(struct hashtable *_htable);
void hashtable_add(struct hashtable *_htable, char *_key, uint32_t _val);
int hashtable_get(struct hashtable *_htable, char *_key);
void hashtable_checkmark(struct hashtable *_htable, char *_key);

#include <stdio.h>
void hashtable_print(struct hashtable *_htable);

#endif