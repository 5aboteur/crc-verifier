#include "hashtable.h"

uint32_t hashtable_hash(char *_str, uint32_t _height)
{
    uint32_t h = 0;
    char *p;

    for (p = _str; *p != '\0'; p++)
        h = h * HASH_MUL + (uint32_t)*p;

    return h % _height;
}

void hashtable_init(struct hashtable *_htable, uint32_t _height)
{
    uint32_t i;

    _htable->table = malloc(sizeof(struct listnode) * _height);

    for (i = 0; i < _height; i++)
        _htable->table[i] = NULL;

    _htable->height = _height;
    _htable->nelems = 0;
}

void hashtable_free(struct hashtable *_htable)
{
    uint32_t i;
    struct listnode *node, *next;

    for (i = 0; i < _htable->height; i++)
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
    uint32_t idx = hashtable_hash(_key, _htable->height);
    struct listnode *node = malloc(sizeof(struct listnode));

    if (node != NULL)
    {
        node->fname = calloc(strlen(_key) + 1, sizeof(char));
        strcpy(node->fname, _key);
        node->crc = _val;
        node->checked = 0;
        node->next = _htable->table[idx];
        _htable->table[idx] = node;
        _htable->nelems++;
    }
}

int hashtable_get(struct hashtable *_htable, char *_key)
{
    uint32_t idx = hashtable_hash(_key, _htable->height);
    struct listnode *node = _htable->table[idx];

    for (; node != NULL; node = node->next) {
        if (strcmp(node->fname, _key) == 0)
            return node->crc;
    }

    return -1;
}

void hashtable_checkmark(struct hashtable *_htable, char *_key)
{
    uint32_t idx = hashtable_hash(_key, _htable->height);
    struct listnode *node = _htable->table[idx];

    for (; node != NULL; node = node->next) {
        if (strcmp(node->fname, _key) == 0)
            node->checked = 1;
    }
}

void hashtable_print(struct hashtable *_htable)
{
    uint32_t i;
    struct listnode *node;

    puts("****************** HASH TABLE ******************");
    for (i = 0; i < _htable->height; i++)
    {
        printf("%3d: ", i);
        node = _htable->table[i];
        for (; node != NULL; node = node->next)
        {
            printf("{[%s]<%d> = %x} ",
                node->fname, node->checked, node->crc);
        }
        printf("\n");
    }
    puts("************************************************");
    printf(" Height => %u\n", _htable->height);
    printf(" Nelems => %u\n", _htable->nelems);
}
