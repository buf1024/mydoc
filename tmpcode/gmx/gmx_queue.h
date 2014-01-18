/*
 * gmx_queue.h
 *
 *  Created on: 2013-12-24
 *      Author: Luo Guochun
 */

#ifndef __GMX_QUEUE_H__
#define __GMX_QUEUE_H__

/*  $OpenBSD: queue.h,v 1.16 2000/09/07 19:47:59 art Exp $  */
/*  $NetBSD: queue.h,v 1.11 1996/05/16 05:17:14 mycroft Exp $   */

/*
 * Copyright (c) 1991, 1993
 *  The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *  @(#)queue.h 8.5 (Berkeley) 8/20/94
 */

/*
 * This file defines five types of data structures: singly-linked lists,
 * lists, simple queues, tail queues, and circular queues.
 *
 *
 * A singly-linked list is headed by a single forward pointer. The elements
 * are singly linked for minimum space and pointer manipulation overhead at
 * the expense of O(n) removal for arbitrary elements. New elements can be
 * added to the list after an existing element or at the head of the list.
 * Elements being removed from the head of the list should use the explicit
 * macro for this purpose for optimum efficiency. A singly-linked list may
 * only be traversed in the forward direction.  Singly-linked lists are ideal
 * for applications with large datasets and few or no removals or for
 * implementing a LIFO queue.
 *
 * A list is headed by a single forward pointer (or an array of forward
 * pointers for a hash table header). The elements are doubly linked
 * so that an arbitrary element can be removed without a need to
 * traverse the list. New elements can be added to the list before
 * or after an existing element or at the head of the list. A list
 * may only be traversed in the forward direction.
 *
 * A simple queue is headed by a pair of pointers, one the head of the
 * list and the other to the tail of the list. The elements are singly
 * linked to save space, so elements can only be removed from the
 * head of the list. New elements can be added to the list before or after
 * an existing element, at the head of the list, or at the end of the
 * list. A simple queue may only be traversed in the forward direction.
 *
 * A tail queue is headed by a pair of pointers, one to the head of the
 * list and the other to the tail of the list. The elements are doubly
 * linked so that an arbitrary element can be removed without a need to
 * traverse the list. New elements can be added to the list before or
 * after an existing element, at the head of the list, or at the end of
 * the list. A tail queue may be traversed in either direction.
 *
 * A circle queue is headed by a pair of pointers, one to the head of the
 * list and the other to the tail of the list. The elements are doubly
 * linked so that an arbitrary element can be removed without a need to
 * traverse the list. New elements can be added to the list before or after
 * an existing element, at the head of the list, or at the end of the list.
 * A circle queue may be traversed in either direction, but has a more
 * complex end of list detection.
 *
 * For details on the use of these macros, see the queue(3) manual page.
 */

/*
 * Singly-linked List definitions.
 */
#define gmx_slist_head(name, type)                      \
struct name {                               \
    struct type *slh_first; /* first element */         \
}

#define gmx_slist_head_initializer(head)                    \
    { NULL }

#ifndef WIN32
#define gmx_slist_entry(type)                       \
struct {                                \
    struct type *sle_next;  /* next element */          \
}
#endif

/*
 * Singly-linked List access methods.
 */
#define gmx_slist_first(head)   ((head)->slh_first)
#define gmx_slist_end(head)     NULL
#define gmx_slist_empty(head)   (gmx_slist_first(head) == gmx_slist_end(head))
#define gmx_slist_next(elm, field)  ((elm)->field.sle_next)

#define gmx_slist_foreach(var, head, field)                 \
    for((var) = gmx_slist_first(head);                  \
        (var) != gmx_slist_end(head);                   \
        (var) = gmx_slist_next(var, field))

/*
 * Singly-linked List functions.
 */
#define gmx_slist_init(head) {                      \
    gmx_slist_first(head) = gmx_slist_end(head);                \
}

#define gmx_slist_insert_after(slistelm, elm, field) do {           \
    (elm)->field.sle_next = (slistelm)->field.sle_next;     \
    (slistelm)->field.sle_next = (elm);             \
} while (0)

#define gmx_slist_insert_head(head, elm, field) do {            \
    (elm)->field.sle_next = (head)->slh_first;          \
    (head)->slh_first = (elm);                  \
} while (0)

#define gmx_slist_remove_head(head, field) do {             \
    (head)->slh_first = (head)->slh_first->field.sle_next;      \
} while (0)

/*
 * List definitions.
 */
#define gmx_list_head(name, type)                       \
struct name {                               \
    struct type *lh_first;  /* first element */         \
}

#define gmx_list_head_initializer(head)                 \
    { NULL }

#define gmx_list_entry(type)                        \
struct {                                \
    struct type *le_next;   /* next element */          \
    struct type **le_prev;  /* address of previous next element */  \
}

/*
 * List access methods
 */
#define gmx_list_first(head)        ((head)->lh_first)
#define gmx_list_end(head)          NULL
#define gmx_list_empty(head)        (gmx_list_first(head) == gmx_list_end(head))
#define gmx_list_next(elm, field)       ((elm)->field.le_next)

#define gmx_list_foreach(var, head, field)                  \
    for((var) = gmx_list_first(head);                   \
        (var)!= gmx_list_end(head);                 \
        (var) = gmx_list_next(var, field))

/*
 * List functions.
 */
#define gmx_list_init(head) do {                        \
    gmx_list_first(head) = gmx_list_end(head);              \
} while (0)

#define gmx_list_insert_after(listelm, elm, field) do {         \
    if (((elm)->field.le_next = (listelm)->field.le_next) != NULL)  \
        (listelm)->field.le_next->field.le_prev =       \
            &(elm)->field.le_next;              \
    (listelm)->field.le_next = (elm);               \
    (elm)->field.le_prev = &(listelm)->field.le_next;       \
} while (0)

#define gmx_list_insert_before(listelm, elm, field) do {            \
    (elm)->field.le_prev = (listelm)->field.le_prev;        \
    (elm)->field.le_next = (listelm);               \
    *(listelm)->field.le_prev = (elm);              \
    (listelm)->field.le_prev = &(elm)->field.le_next;       \
} while (0)

#define gmx_list_insert_head(head, elm, field) do {             \
    if (((elm)->field.le_next = (head)->lh_first) != NULL)      \
        (head)->lh_first->field.le_prev = &(elm)->field.le_next;\
    (head)->lh_first = (elm);                   \
    (elm)->field.le_prev = &(head)->lh_first;           \
} while (0)

#define gmx_list_remove(elm, field) do {                    \
    if ((elm)->field.le_next != NULL)               \
        (elm)->field.le_next->field.le_prev =           \
            (elm)->field.le_prev;               \
    *(elm)->field.le_prev = (elm)->field.le_next;           \
} while (0)

#define gmx_list_replace(elm, elm2, field) do {             \
    if (((elm2)->field.le_next = (elm)->field.le_next) != NULL) \
        (elm2)->field.le_next->field.le_prev =          \
            &(elm2)->field.le_next;             \
    (elm2)->field.le_prev = (elm)->field.le_prev;           \
    *(elm2)->field.le_prev = (elm2);                \
} while (0)

#define gmx_list_pre(elm, field) \
    (((elm)->field.le_pre) != &elm ? *((elm)->field.le_pre) : NULL)

/*
 * Simple queue definitions.
 */
#define gmx_simpleq_head(name, type)                    \
struct name {                               \
    struct type *sqh_first; /* first element */         \
    struct type **sqh_last; /* addr of last next element */     \
}

#define gmx_simpleq_head_initializer(head)                  \
    { NULL, &(head).sqh_first }

#define gmx_simpleq_entry(type)                     \
struct {                                \
    struct type *sqe_next;  /* next element */          \
}

/*
 * Simple queue access methods.
 */
#define gmx_simpleq_first(head)     ((head)->sqh_first)
#define gmx_simpleq_end(head)       NULL
#define gmx_simpleq_empty(head)     (gmx_simpleq_first(head) == gmx_simpleq_end(head))
#define gmx_simpleq_next(elm, field)    ((elm)->field.sqe_next)

#define gmx_simpleq_foreach(var, head, field)               \
    for((var) = gmx_simpleq_first(head);                \
        (var) != gmx_simpleq_end(head);                 \
        (var) = gmx_simpleq_next(var, field))

/*
 * Simple queue functions.
 */
#define gmx_simpleq_init(head) do {                     \
    (head)->sqh_first = NULL;                   \
    (head)->sqh_last = &(head)->sqh_first;              \
} while (0)

#define gmx_simpleq_insert_head(head, elm, field) do {          \
    if (((elm)->field.sqe_next = (head)->sqh_first) == NULL)    \
        (head)->sqh_last = &(elm)->field.sqe_next;      \
    (head)->sqh_first = (elm);                  \
} while (0)

#define gmx_simpleq_insert_tail(head, elm, field) do {          \
    (elm)->field.sqe_next = NULL;                   \
    *(head)->sqh_last = (elm);                  \
    (head)->sqh_last = &(elm)->field.sqe_next;          \
} while (0)

#define gmx_simpleq_insert_after(head, listelm, elm, field) do {        \
    if (((elm)->field.sqe_next = (listelm)->field.sqe_next) == NULL)\
        (head)->sqh_last = &(elm)->field.sqe_next;      \
    (listelm)->field.sqe_next = (elm);              \
} while (0)

#define gmx_simpleq_remove_head(head, elm, field) do {          \
    if (((head)->sqh_first = (elm)->field.sqe_next) == NULL)    \
        (head)->sqh_last = &(head)->sqh_first;          \
} while (0)

/*
 * Tail queue definitions.
 */
#define gmx_tailq_head(name, type)                      \
struct name {                               \
    struct type *tqh_first; /* first element */         \
    struct type **tqh_last; /* addr of last next element */     \
}

#define gmx_tailq_head_initializer(head)                    \
    { NULL, &(head).tqh_first }

#define gmx_tailq_entry(type)                       \
struct {                                \
    struct type *tqe_next;  /* next element */          \
    struct type **tqe_prev; /* address of previous next element */  \
}

/*
 * tail queue access methods
 */
#define gmx_tailq_first(head)       ((head)->tqh_first)
#define gmx_tailq_end(head)         NULL
#define gmx_tailq_next(elm, field)      ((elm)->field.tqe_next)
#define gmx_tailq_last(head, headname)                  \
    (*(((struct headname *)((head)->tqh_last))->tqh_last))
/* XXX */
#define gmx_tailq_prev(elm, headname, field)                \
    (*(((struct headname *)((elm)->field.tqe_prev))->tqh_last))
#define gmx_tailq_empty(head)                       \
    (gmx_tailq_first(head) == gmx_tailq_end(head))

#define gmx_tailq_foreach(var, head, field)                 \
    for((var) = gmx_tailq_first(head);                  \
        (var) != gmx_tailq_end(head);                   \
        (var) = gmx_tailq_next(var, field))

#define gmx_tailq_foreach_reverse(var, head, headname, field)       \
    for((var) = gmx_tailq_last(head, headname);             \
        (var) != gmx_tailq_end(head);                   \
        (var) = gmx_tailq_prev(var, headname, field))

/*
 * Tail queue functions.
 */
#define gmx_tailq_init(head) do {                       \
    (head)->tqh_first = NULL;                   \
    (head)->tqh_last = &(head)->tqh_first;              \
} while (0)

#define gmx_tailq_insert_head(head, elm, field) do {            \
    if (((elm)->field.tqe_next = (head)->tqh_first) != NULL)    \
        (head)->tqh_first->field.tqe_prev =         \
            &(elm)->field.tqe_next;             \
    else                                \
        (head)->tqh_last = &(elm)->field.tqe_next;      \
    (head)->tqh_first = (elm);                  \
    (elm)->field.tqe_prev = &(head)->tqh_first;         \
} while (0)

#define gmx_tailq_insert_tail(head, elm, field) do {            \
    (elm)->field.tqe_next = NULL;                   \
    (elm)->field.tqe_prev = (head)->tqh_last;           \
    *(head)->tqh_last = (elm);                  \
    (head)->tqh_last = &(elm)->field.tqe_next;          \
} while (0)

#define gmx_tailq_insert_after(head, listelm, elm, field) do {      \
    if (((elm)->field.tqe_next = (listelm)->field.tqe_next) != NULL)\
        (elm)->field.tqe_next->field.tqe_prev =         \
            &(elm)->field.tqe_next;             \
    else                                \
        (head)->tqh_last = &(elm)->field.tqe_next;      \
    (listelm)->field.tqe_next = (elm);              \
    (elm)->field.tqe_prev = &(listelm)->field.tqe_next;     \
} while (0)

#define gmx_tailq_insert_before(listelm, elm, field) do {           \
    (elm)->field.tqe_prev = (listelm)->field.tqe_prev;      \
    (elm)->field.tqe_next = (listelm);              \
    *(listelm)->field.tqe_prev = (elm);             \
    (listelm)->field.tqe_prev = &(elm)->field.tqe_next;     \
} while (0)

#define gmx_tailq_remove(head, elm, field) do {             \
    if (((elm)->field.tqe_next) != NULL)                \
        (elm)->field.tqe_next->field.tqe_prev =         \
            (elm)->field.tqe_prev;              \
    else                                \
        (head)->tqh_last = (elm)->field.tqe_prev;       \
    *(elm)->field.tqe_prev = (elm)->field.tqe_next;         \
} while (0)

#define gmx_tailq_replace(head, elm, elm2, field) do {          \
    if (((elm2)->field.tqe_next = (elm)->field.tqe_next) != NULL)   \
        (elm2)->field.tqe_next->field.tqe_prev =        \
            &(elm2)->field.tqe_next;                \
    else                                \
        (head)->tqh_last = &(elm2)->field.tqe_next;     \
    (elm2)->field.tqe_prev = (elm)->field.tqe_prev;         \
    *(elm2)->field.tqe_prev = (elm2);               \
} while (0)

/*
 * Circular queue definitions.
 */
#define gmx_circleq_head(name, type)                    \
struct name {                               \
    struct type *cqh_first;     /* first element */     \
    struct type *cqh_last;      /* last element */      \
}

#define gmx_circleq_head_initializer(head)                  \
    { gmx_circleq_end(&head), gmx_circleq_end(&head) }

#define gmx_circleq_entry(type)                     \
struct {                                \
    struct type *cqe_next;      /* next element */      \
    struct type *cqe_prev;      /* previous element */      \
}

/*
 * Circular queue access methods
 */
#define gmx_circleq_first(head)     ((head)->cqh_first)
#define gmx_circleq_last(head)      ((head)->cqh_last)
#define gmx_circleq_end(head)       ((void *)(head))
#define gmx_circleq_next(elm, field)    ((elm)->field.cqe_next)
#define gmx_circleq_prev(elm, field)    ((elm)->field.cqe_prev)
#define gmx_circleq_empty(head)                     \
    (gmx_circleq_first(head) == gmx_circleq_end(head))

#define gmx_circleq_foreach(var, head, field)               \
    for((var) = gmx_circleq_first(head);                \
        (var) != gmx_circleq_end(head);                 \
        (var) = gmx_circleq_next(var, field))

#define gmx_circleq_foreach_reverse(var, head, field)           \
    for((var) = gmx_circleq_last(head);                 \
        (var) != gmx_circleq_end(head);                 \
        (var) = gmx_circleq_prev(var, field))

/*
 * Circular queue functions.
 */
#define gmx_circleq_init(head) do {                     \
    (head)->cqh_first = gmx_circleq_end(head);              \
    (head)->cqh_last = gmx_circleq_end(head);               \
} while (0)

#define gmx_circleq_insert_after(head, listelm, elm, field) do {        \
    (elm)->field.cqe_next = (listelm)->field.cqe_next;      \
    (elm)->field.cqe_prev = (listelm);              \
    if ((listelm)->field.cqe_next == gmx_circleq_end(head))     \
        (head)->cqh_last = (elm);               \
    else                                \
        (listelm)->field.cqe_next->field.cqe_prev = (elm);  \
    (listelm)->field.cqe_next = (elm);              \
} while (0)

#define gmx_circleq_insert_before(head, listelm, elm, field) do {       \
    (elm)->field.cqe_next = (listelm);              \
    (elm)->field.cqe_prev = (listelm)->field.cqe_prev;      \
    if ((listelm)->field.cqe_prev == gmx_circleq_end(head))     \
        (head)->cqh_first = (elm);              \
    else                                \
        (listelm)->field.cqe_prev->field.cqe_next = (elm);  \
    (listelm)->field.cqe_prev = (elm);              \
} while (0)

#define gmx_circleq_insert_head(head, elm, field) do {          \
    (elm)->field.cqe_next = (head)->cqh_first;          \
    (elm)->field.cqe_prev = gmx_circleq_end(head);          \
    if ((head)->cqh_last == gmx_circleq_end(head))          \
        (head)->cqh_last = (elm);               \
    else                                \
        (head)->cqh_first->field.cqe_prev = (elm);      \
    (head)->cqh_first = (elm);                  \
} while (0)

#define gmx_circleq_insert_tail(head, elm, field) do {          \
    (elm)->field.cqe_next = gmx_circleq_end(head);          \
    (elm)->field.cqe_prev = (head)->cqh_last;           \
    if ((head)->cqh_first == gmx_circleq_end(head))         \
        (head)->cqh_first = (elm);              \
    else                                \
        (head)->cqh_last->field.cqe_next = (elm);       \
    (head)->cqh_last = (elm);                   \
} while (0)

#define gmx_circleq_remove(head, elm, field) do {               \
    if ((elm)->field.cqe_next == gmx_circleq_end(head))         \
        (head)->cqh_last = (elm)->field.cqe_prev;       \
    else                                \
        (elm)->field.cqe_next->field.cqe_prev =         \
            (elm)->field.cqe_prev;              \
    if ((elm)->field.cqe_prev == gmx_circleq_end(head))         \
        (head)->cqh_first = (elm)->field.cqe_next;      \
    else                                \
        (elm)->field.cqe_prev->field.cqe_next =         \
            (elm)->field.cqe_next;              \
} while (0)

#define gmx_circleq_replace(head, elm, elm2, field) do {            \
    if (((elm2)->field.cqe_next = (elm)->field.cqe_next) ==     \
        gmx_circleq_end(head))                      \
        (head).cqh_last = (elm2);               \
    else                                \
        (elm2)->field.cqe_next->field.cqe_prev = (elm2);    \
    if (((elm2)->field.cqe_prev = (elm)->field.cqe_prev) ==     \
        gmx_circleq_end(head))                      \
        (head).cqh_first = (elm2);              \
    else                                \
        (elm2)->field.cqe_prev->field.cqe_next = (elm2);    \
} while (0)

#ifdef __cplusplus
extern "C" {
#endif



#ifdef __cplusplus
}
#endif

#endif /* __GMX_QUEUE_H__ */
