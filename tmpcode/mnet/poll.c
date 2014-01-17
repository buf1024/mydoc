/*
 * poll.c
 *
 *  Created on: 2012-12-14
 *      Author: buf1024@gmail.com
 */

#include "evt.h"
#include <poll.h>

struct evt_be_pool
{
    size_t cnt;
    struct pollfd evts[EVT_MAX_EVENT];
};

int be_evt_init(struct event* evt)
{
    struct evt_be_pool* ep = (struct evt_be_pool*) malloc(sizeof(*ep));
    if (!ep)
        return -1;
    memset(ep, 0, sizeof(*ep));
    evt->data = ep;
    return 0;
}
int be_evt_destroy(struct event* evt)
{
    free(evt->data);
    return 0;
}

int be_evt_poll(struct event* evt)
{
    struct evt_be_pool* ep = (struct evt_be_pool*) evt->data;
    struct list* fd = evt->fds;
    struct list_item* itm = NULL;
    struct evt_poll* evtp = NULL;
    int epr = 0;
    int i = 0;

    epr = poll(ep->evts, ep->cnt,
            evt->to->timeout ? evt->to->timeout : evt->defto); // TODO if timeout 0?
    if (epr > 0) {
        for (; i < ep->cnt; i++) {
            itm = list_pull(fd, &(ep->evts[i].fd));
            if (!itm)
                return -1;

            evtp = (struct evt_poll*) itm->data;
            evtp->evt = 0;

            if (ep->evts[i].revents & POLLIN)
                evtp->evt |= EVT_READ;
            if (ep->evts[i].revents & POLLOUT)
                evtp->evt |= EVT_WRITE;
            if (ep->evts[i].revents & POLLHUP)
                evtp->evt |= EVT_HUP;
            if (ep->evts[i].revents & POLLERR)
                evtp->evt |= EVT_ERROR;

        }
    }

    return epr;
}

int be_evt_add(struct event* evt, int fd, int mask)
{
    struct evt_be_pool* ep = (struct evt_be_pool*) evt->data;
    int i = 0;
    for (; i < ep->cnt; i++) {
        if (fd == ep->evts[i].fd) {
            break;
        }
    }
    if (i >= ep->cnt) {
        if (ep->cnt < EVT_MAX_EVENT) {
            ep->evts[ep->cnt].fd = fd;
            ep->evts[ep->cnt].revents = 0;
            ep->cnt++;
        } else {
            return -1;
        }
    }

    if (mask & EVT_READ)
        ep->evts[i].events |= POLLIN;
    if (mask & EVT_WRITE)
        ep->evts[i].events |= POLLOUT;
    if (mask & EVT_HUP)
        ep->evts[i].events |= POLLHUP;
    if (mask & EVT_ERROR)
        ep->evts[i].events |= POLLERR;

    return 0;
}

int be_evt_del(struct event* evt, int fd)
{
    struct evt_be_pool* ep = (struct evt_be_pool*) evt->data;
    int i = 0;
    for (; i < ep->cnt; i++) {
        if (fd == ep->evts[i].fd) {
            break;
        }
    }
    if (i < ep->cnt) {
        //TODO if size = 0?
        memmove(&(ep->evts[i]), &(ep->evts[i + 1]), (ep->cnt - i - 1)*sizeof(struct pollfd));
        ep->cnt--;
    }

    return 0;
}
