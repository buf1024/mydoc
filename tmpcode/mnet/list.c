/*
 * list.c
 *
 *  Created on: 2012-12-14
 *      Author: buf1024@gmail.com
 */

#include <stdlib.h>
#include "list.h"

struct list* list_new()
{
    struct list* lst = (struct list*) malloc(sizeof(*lst));
    if (lst) {
        lst->data = NULL;

        lst->cmp = NULL;
        lst->alloc = NULL;
        lst->free = NULL;

        lst->cnt = 0;
        lst->fst_itm = NULL;
        lst->lst_itm = NULL;
    }
    return lst;
}
int list_del(struct list* lst)
{
    struct list_item* litm = lst->fst_itm;
    while (litm) {
        list_rm(lst, litm);
        litm = lst->fst_itm;
    }
    return 0;
}


int list_push_front(struct list* lst, void* data)
{
    struct list_item* itm = (struct list_item*) malloc(sizeof(*itm));
    if (!itm)
        return -1;

    itm->data = lst->alloc? lst->alloc(data):data;
    itm->nxt_itm = NULL;
    itm->pre_itm = NULL;

    if (lst->cnt) {
        itm->nxt_itm = lst->fst_itm;
        lst->fst_itm->pre_itm = itm;
        lst->fst_itm = itm;
    } else {
        lst->fst_itm = itm;
        lst->lst_itm = itm;
    }
    lst->cnt++;
    return 0;
}
int list_push_tail(struct list* lst, void* data)
{
    struct list_item* itm = (struct list_item*) malloc(sizeof(*itm));
    if (!itm)
        return -1;

    itm->data = lst->alloc? lst->alloc(data):data;
    itm->nxt_itm = NULL;
    itm->pre_itm = NULL;

    if (lst->cnt) {
        itm->pre_itm = lst->lst_itm;
        lst->lst_itm->nxt_itm = itm;
        lst->lst_itm = itm;
    } else {
        lst->fst_itm = itm;
        lst->lst_itm = itm;
    }
    lst->cnt++;
    return 0;
}
int list_push_front_of(struct list* lst, struct list_item* front, void* data)
{
    struct list_item* litm = lst->fst_itm;

    struct list_item* itm = (struct list_item*) malloc(sizeof(*itm));
    if (!itm)
        return -1;

    itm->data = lst->alloc? lst->alloc(data):data;
    itm->nxt_itm = NULL;
    itm->pre_itm = NULL;
    
    while (litm) {
        if (litm == front) {
            itm->nxt_itm = front;
            itm->pre_itm = front->pre_itm;
            front->pre_itm = itm;
            if (front == lst->fst_itm) {
                lst->fst_itm = itm;
            }
            lst->cnt++;
            return 0;
        }
        litm = litm->nxt_itm;
    }
    return -1;
}
int list_push_tail_of(struct list* lst, struct list_item* tail, void* data)
{
    struct list_item* litm = lst->fst_itm;

    struct list_item* itm = (struct list_item*) malloc(sizeof(*itm));
    if (!itm)
        return -1;

    itm->data = lst->alloc? lst->alloc(data):data;
    itm->nxt_itm = NULL;
    itm->pre_itm = NULL;

    while (litm) {
        if (litm == tail) {
            itm->nxt_itm = tail->nxt_itm;
            itm->pre_itm = tail;
            tail->nxt_itm = itm;
            if (tail == lst->lst_itm) {
                lst->lst_itm = itm;
            }
            lst->cnt++;
            return 0;
        }
        litm = litm->nxt_itm;
    }
    return -1;
}

struct list_item* list_pull(struct list* lst, void* needle)
{
    struct list_item* litm = lst->fst_itm;
    int flags = 0;
    while (litm) {
        if (lst->cmp) {
            flags = lst->cmp(litm->data, needle) == 0;
        } else {
            flags = (litm->data == needle);
        }
        if (flags) {
            return litm;
        }
        litm = litm->nxt_itm;
    }
    return NULL ;
}
int list_drop(struct list* lst, void* needle)
{
    struct list_item* litm = lst->fst_itm;
    int flags = 0;
    while (litm) {
        if (lst->cmp) {
            flags = lst->cmp(litm->data, needle) == 0;
        } else {
            flags = (litm->data == needle);
        }
        if (flags) {
            return list_rm(lst, litm);
        }
        litm = litm->nxt_itm;
    }
    return -1;
}
int list_rm(struct list* lst, struct list_item* itm)
{
    if (itm == lst->fst_itm) {
        lst->fst_itm = itm->nxt_itm;
        if(lst->fst_itm){
            lst->fst_itm->pre_itm = NULL;
        }
    }
    if (itm == lst->lst_itm) {
        lst->lst_itm = itm->pre_itm;
        if(lst->lst_itm){
            lst->lst_itm->nxt_itm = NULL;
        }
    }
    if (itm->pre_itm) {
        itm->pre_itm->nxt_itm = itm->nxt_itm;
    }
    if (itm->nxt_itm) {
        itm->nxt_itm->pre_itm = itm->pre_itm;
    }
    if(lst->free){
        lst->free(itm->data);
    }
    free(itm);
    lst->cnt--;
    return 0;
}
int list_size(struct list* lst)
{
    return lst->cnt;
}

struct list_iterator* list_iterator_new(struct list* lst, int direct)
{
    struct list_iterator* iter = (struct list_iterator*) malloc(sizeof(*iter));
    if (iter) {
        iter->lst = lst;
        iter->direct = direct;
        iter->cur_itm = NULL;
    }
    return iter;
}
int list_iterator_del(struct list_iterator* iter)
{
    free(iter);

    return 0;
}

struct list_item* list_bebin(struct list_iterator* iter)
{
    if (iter->direct == 0) {
        iter->cur_itm = iter->lst->fst_itm;
    } else {
        iter->cur_itm = iter->lst->lst_itm;
    }
    return iter->cur_itm;
}
struct list_item* list_next(struct list_iterator* iter)
{
    if (iter->cur_itm) {
        if (iter->direct == 0) {
            iter->cur_itm = iter->cur_itm->nxt_itm;
        } else {
            iter->cur_itm = iter->cur_itm->pre_itm;
        }
    }
    return iter->cur_itm;
}
int list_end(struct list_iterator* iter)
{
    if (iter->cur_itm == NULL )
        return 1;
    return 0;
}

int list_travel(struct list* lst, int direct, list_travel_t fn)
{
    struct list_item* itm = NULL;
    int ret = 0;

    if(direct == 0) itm = lst->fst_itm;
    else itm = lst->lst_itm;

    while (itm) {
        ret = fn(itm->data);
        if(!ret) return ret;
        if(direct == 0) itm = itm->nxt_itm;
        else itm = itm->pre_itm;
    }

    return 0 ;
}
