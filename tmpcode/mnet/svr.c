/*
 * main.c
 *
 *  Created on: 2012-12-14
 *      Author: buf1024@gmail.com
 */


#include <assert.h>
#include "evt.h"
#include "sock.h"

int mycb(struct event* evt, int fd, int mask, void* args)
{
    return 0;
}

int main(int argc, char **argv)
{
    struct event* evt;

    int fd;

    fd = tcp_svr(NULL, 95277);

    evt = evt_new();
    assert(evt != NULL);
    evt_add(evt, fd, EVT_SOCK, EVT_RDWR, mycb, NULL);

    evt_loop(evt);

    evt_del(evt);

    return 0;
}

