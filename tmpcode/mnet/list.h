/*
 * list.h
 *
 *  Created on: 2012-12-14
 *      Author: buf1024@gmail.com
 */

#ifndef __48SLOTS_LIST_H__
#define __48SLOTS_LIST_H__

#ifdef __cplusplus
extern "C" {
#endif

//not validate the arguments.
//invoker must respond for passing the correct arguments

typedef int (*list_cmp_t)(void*, void*);
typedef void* (*list_alloc_t)(void*);
typedef void (*list_free_t)(void*);

typedef int (*list_travel_t)(void*);

struct list_item;
struct list
{
    void* data; //head data

    list_cmp_t cmp;       //data cmp
    list_alloc_t alloc;   //data allocator
    list_free_t free;     //data deallocator

    size_t cnt;
    struct list_item* fst_itm;
    struct list_item* lst_itm;
};

struct list_item
{
    void* data;

    struct list_item* nxt_itm;
    struct list_item* pre_itm;
};

struct list_iterator
{
    struct list* lst;
    int direct;
    struct list_item* cur_itm;
};

//list
struct list* list_new();
int list_del(struct list* lst);

int list_push_front(struct list* lst, void* data);
int list_push_tail(struct list* lst, void* data);
int list_push_front_of(struct list* lst, struct list_item* front, void* data);
int list_push_tail_of(struct list* lst, struct list_item* tail, void* data);

struct list_item* list_pull(struct list* lst, void* needle);
int list_drop(struct list* lst, void* needle);
int list_rm(struct list* lst, struct list_item* itm);

int list_size(struct list* lst);

struct list_iterator* list_iterator_new(struct list* list, int direct);
int list_iterator_del(struct list_iterator* iter);

struct list_item* list_bebin(struct list_iterator* iter);
struct list_item* list_next(struct list_iterator* iter);
int list_end(struct list_iterator* iter);

int list_travel(struct list* lst, int direct, list_travel_t fn);

#define list_foreach(itm, iter) \
    for(itm = list_bebin(iter); !list_end(iter); itm = list_next(iter))

#define list_set_attr(lst, lsdata, fncmp, fnalloc, fnfree) \
    do{lst->data = lsdata;lst->cmp = fncmp;lst->alloc = fnalloc;lst->free = fnfree;}while(0)

#ifdef __cplusplus
}
#endif

#endif /* __48SLOTS_LIST_H__ */
