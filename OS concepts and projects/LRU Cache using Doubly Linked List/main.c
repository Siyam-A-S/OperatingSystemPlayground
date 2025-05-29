
#include <stdlib.h>

struct node {
    int key;
    int val;
    struct node *next;
    struct node *prev;
};

typedef struct {
    int cap;
    int count;
} LRUCache;

struct node *head;
struct node *tail;

struct node* rem[10001];
LRUCache* lRUCacheCreate(int capacity) {
    LRUCache *cache = malloc(sizeof(LRUCache));
    cache->cap = capacity;
    cache->count = 0;
    head = malloc(sizeof(struct node));
    tail = malloc(sizeof(struct node));

    head->next =tail;
    tail->prev = head;

    for (int i = 0; i< 10001; i++) {
        rem[i] = NULL;
    }

    return cache;
}

 void del(struct node *curr) {
    curr->prev->next = curr->next;
    curr->next->prev = curr->prev;
 }

 void add(struct node *curr) {
    curr->next = head->next;
    curr->prev = head;

    head->next->prev = curr;
    head->next = curr;
 }

 int lRUCacheGet(LRUCache* obj, int key) {
    if (obj->count == 0) 
        return -1;
    if (rem[key] == NULL)
        return -1;
    
    int val;
    struct node *curr = rem[key];
    val = curr->val;
    del(curr);
    add(curr);
    return val;
}

void LRUCachePut(LRUCache* obj, int key, int value) {
    printf("key=%d\n", key);
    if (rem[key] != NULL) {
    struct node *curr = rem[key];
    }
}