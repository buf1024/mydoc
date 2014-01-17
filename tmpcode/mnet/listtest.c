/*
 * listtest.c
 *
 *  Created on: 2012-12-14
 *      Author: buf1024@gmail.com
 */

#include <stdlib.h>
#include <stdio.h>
#include "list.h"

static int mycmp(void* t1, void* t2)
{
    int i1 = *(int*)t1;
    int i2 = *(int*)t2;
    if(i1 > i2) return 1;
    if(i1 < i2) return -1;
    return 0;
}

int main(int argc, char **argv)
{
    struct list* lst = NULL;
    struct list_iterator* iter = NULL;
    struct list_item* itm = NULL;
    int tmp = 0;
    int data[10] = { 0 };
    int i = 0;

    lst = list_new();

    list_set_attr(lst, NULL, mycmp, NULL, NULL);

    iter = list_iterator_new(lst, 0);

    for (i = 0; i < 10; i++) {
        data[i] = i+1;
        list_push_tail(lst, data + i);
    }

    printf("size: %lu\n", lst->cnt);

    list_foreach(itm, iter){
        tmp = (*(int*)(itm->data));
        printf("%d\n", tmp);
    }
    printf("\n");

    list_drop(lst, data + 2);
    list_drop(lst, data + 0);
    list_drop(lst, data + 9);

    printf("size: %lu\n", lst->cnt);
    list_foreach(itm, iter)
    {
        tmp = (*(int*) (itm->data));
        printf("%d\n", tmp);
    }
    printf("\n");

    list_push_front(lst, data+2);
    list_push_tail(lst, data+0);

    printf("size: %lu\n", lst->cnt);

    list_foreach(itm, iter)
    {
        tmp = (*(int*) (itm->data));
        printf("%d\n", tmp);
    }

    itm = list_pull(lst, data+5);
    if(itm){
        printf("found: %d\n", *(int*)itm->data);
    }else{
        printf("not found");
    }

    list_iterator_del(iter);
    list_del(lst);

    return 0;
}

