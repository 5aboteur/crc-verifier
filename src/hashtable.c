#include "hashtable.h"

uint32_t hashtable_hash(char *_str, uint32_t _tablesz)
{
    uint32_t h = 0;
    char *p;

    for (p = _str; *p != '\0'; p++)
        h = h * HASH_MUL + (uint32_t)*p;

    return h % _tablesz;
}

void hashtable_init(struct hashtable *_htable, uint32_t _tablesz)
{
    uint32_t i;

    _htable->table = malloc(sizeof(struct listnode) * _tablesz);

    for (i = 0; i < _tablesz; i++)
        _htable->table[i] = NULL;

    _htable->size = _tablesz;
}

void hashtable_free(struct hashtable *_htable)
{
    uint32_t i;
    struct listnode *node, *next;

    for (i = 0; i < _htable->size; i++)
    {
        node = _htable->table[i];
        while (node)
        {
            next = node->next;
            free(node->fname);
            free(node);
            node = next;
        }
    }

    free(_htable->table);
}

void hashtable_add(struct hashtable *_htable, char *_key, uint32_t _val)
{
    uint32_t idx = hashtable_hash(_key, _htable->size);
    struct listnode *node = malloc(sizeof(struct listnode));

    if (node != NULL)
    {
        node->fname = calloc(strlen(_key) + 1, sizeof(char));
        strcpy(node->fname, _key);
        node->crc = _val;
        node->next = _htable->table[idx];
        _htable->table[idx] = node;
    }
}

int hashtable_get(struct hashtable *_htable, char *_key)
{
    uint32_t idx = hashtable_hash(_key, _htable->size);
    struct listnode *node = _htable->table[idx];

    for (; node != NULL; node = node->next) {
        if (strcmp(node->fname, _key) == 0)
            return node->crc;
    }

    return -1;
}

void hashtable_print(struct hashtable *_htable)
{
    uint32_t i;
    struct listnode *node;

    puts("****************** HASH TABLE ******************");
    for (i = 0; i < _htable->size; i++)
    {
        printf("%3d: ", i);
        node = _htable->table[i];
        for (; node != NULL; node = node->next)
        {
            printf("{[%s] = %x} ", node->fname, node->crc);
        }
        printf("\n");
    }
    puts("************************************************");
    printf(" : %u element(s)\n", _htable->size);
}
