/*
 * epoll.c
 *
 *  Created on: 2012-12-14
 *      Author: buf1024@gmail.com
 */

#include "evt.h"
#include <sys/epoll.h>
#include <string.h>

#define EVT_EPOLL_SIZE 1024

struct evt_be_epool
{
    int epfd;
    size_t cnt;
    struct epoll_event evts[EVT_MAX_EVENT];
};

int be_evt_init(struct event* evt)
{
    struct evt_be_epool* ep = (struct evt_be_epool*) malloc(sizeof(*ep));
    if (!ep)
        return -1;
    memset(ep, 0, sizeof(*ep));
    ep->epfd = epoll_create(EVT_EPOLL_SIZE);
    if (ep->epfd <= 0) {
        free(ep);
        return -1;
    }
    evt->data = ep;
    return 0;
}
int be_evt_destroy(struct event* evt)
{
    struct evt_be_epool* ep = (struct evt_be_epool*) evt->data;
    close(ep->epfd);
    free(evt->data);
    return 0;
}

int be_evt_poll(struct event* evt)
{
    struct evt_be_epool* ep = (struct evt_be_epool*) evt->data;
    struct list* fd = evt->fds;
    struct list_item* itm = NULL;
    struct evt_poll* evtp = NULL;
    int epr = 0;
    int i = 0;

    epr = epoll_wait(ep->epfd, ep->evts, ep->cnt,
            evt->to->timeout ? evt->to->timeout : evt->defto);

    if (epr > 0) {
        for (; i < ep->cnt; i++) {
            itm = list_pull(fd, &(ep->evts[i].data.fd));
            if (!itm)
                return -1;

            evtp = (struct evt_poll*) itm->data;
            evtp->evt = 0;

            if (ep->evts[i].events & EPOLLIN)
                evtp->evt |= EVT_READ;
            if (ep->evts[i].events & EPOLLOUT)
                evtp->evt |= EVT_WRITE;
            if (ep->evts[i].events & EPOLLHUP)
                evtp->evt |= EVT_HUP;
            if (ep->evts[i].events & EPOLLERR)
                evtp->evt |= EVT_ERROR;

        }
    }

    return epr;
}

int be_evt_add(struct event* evt, int fd, int mask)
{
    struct evt_be_epool* ep = (struct evt_be_epool*) evt->data;
    struct epoll_event* epevt = NULL;
    int op = EPOLL_CTL_ADD;
    int i = 0;
    for (; i < ep->cnt; i++) {
        if (fd == ep->evts[i].data.fd) {
            epevt = &(ep->evts[i]);
            op = EPOLL_CTL_MOD;
            break;
        }
    }
    if (!epevt) {
        if (ep->cnt < EVT_MAX_EVENT) {
            epevt = &(ep->evts[ep->cnt]);
            memset(epevt, 0, sizeof(*epevt));
            epevt->data.fd = fd;
            op = EPOLL_CTL_ADD;
            ep->cnt++;
        } else {
            return -1;
        }
    }

    if (mask & EVT_READ)
        epevt->events |= EPOLLIN;
    if (mask & EVT_WRITE)
        epevt->events |= EPOLLOUT;
    if (mask & EVT_HUP)
        epevt->events |= EPOLLHUP;
    if (mask & EVT_ERROR)
        epevt->events |= EPOLLERR;

    if (epoll_ctl(ep->epfd, op, fd, epevt) < 0)
        return -1;

    return 0;
}

int be_evt_del(struct event* evt, int fd)
{
    struct evt_be_epool* ep = (struct evt_be_epool*) evt->data;
    struct epoll_event* epevt = NULL;
    int i = 0;
    for (; i < ep->cnt; i++) {
        if (fd == ep->evts[i].data.fd) {
            epevt = &(ep->evts[i]);
            break;
        }
    }
    if (i < ep->cnt) {
        if (epoll_ctl(ep->epfd, EPOLL_CTL_DEL, fd, epevt) < 0)
            return -1;
        //TODO if size = 0?
        memmove(&(ep->evts[i]), &(ep->evts[i + 1]),
                (ep->cnt - i - 1) * sizeof(struct epoll_event));
        ep->cnt--;
    }

    return 0;
}
