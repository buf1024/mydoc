/*
 * select.c
 *
 *  Created on: 2012-12-14
 *      Author: buf1024@gmail.com
 */

#include "evt.h"
#include <string.h>
#include <sys/select.h>

struct evt_be_select
{
    int maxfd;
    fd_set rfd, wfd, efd;
    fd_set erfd, ewfd, eefd;
};

int be_evt_init(struct event* evt)
{
    struct evt_be_select* ep = (struct evt_be_select*) malloc(sizeof(*ep));
    if (!ep)
        return -1;
    memset(ep, 0, sizeof(*ep));

    FD_ZERO(&ep->rfd);
    FD_ZERO(&ep->wfd);

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
    struct evt_be_select* ep = (struct evt_be_select*) evt->data;
    struct list* fd = evt->fds;
    struct list_item* itm = NULL;
    struct evt_poll* evtp = NULL;

    struct timeval tv = {0};

    int epr = 0;
    int i = 0;

    memcpy(&ep->erfd, &ep->rfd, sizeof(fd_set));
    memcpy(&ep->ewfd, &ep->wfd, sizeof(fd_set));
    memcpy(&ep->eefd, &ep->efd, sizeof(fd_set));

    i = evt->to->timeout ? evt->to->timeout : evt->defto;
    tv.tv_sec = i / 1000;
    tv.tv_usec = i % 1000;

    epr = select(ep->maxfd + 1, &ep->erfd, &ep->ewfd, &ep->eefd, &tv); // TODO if timeout 0?
    if (epr > 0) {
        for (; i <= ep->maxfd; i++) {
            itm = list_pull(fd, &i);
            if (!itm)
                continue;

            evtp = (struct evt_poll*) itm->data;
            evtp->evt = 0;

            if (FD_ISSET(i, &ep->erfd))
                evtp->evt |= EVT_READ;
            if (FD_ISSET(i, &ep->ewfd))
                evtp->evt |= EVT_WRITE;
            if (FD_ISSET(i, &ep->eefd)) {
                evtp->evt |= EVT_HUP;
                evtp->evt |= EVT_ERROR;
            }

        }
    }

    return epr;
}

int be_evt_add(struct event* evt, int fd, int mask)
{
    struct evt_be_select* ep = (struct evt_be_select*) evt->data;

    if (mask & EVT_READ)
        FD_SET(fd, &ep->rfd);
    if (mask & EVT_WRITE)
        FD_SET(fd, &ep->wfd);
    if ((mask & EVT_HUP) || (mask & EVT_ERROR))
        FD_SET(fd, &ep->efd);

    if (fd > ep->maxfd)
        ep->maxfd = fd;

    return 0;
}

int be_evt_del(struct event* evt, int fd)
{
    struct evt_be_select* ep = (struct evt_be_select*) evt->data;

    FD_CLR(fd, &ep->rfd);
    FD_CLR(fd, &ep->wfd);
    FD_CLR(fd, &ep->efd);

    return 0;
}
