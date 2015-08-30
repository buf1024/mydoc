/*
 * clt.c
 *
 *  Created on: 2012-12-14
 *      Author: buf1024@gmail.com
 */

#include "evt.h"
#include "sock.h"
#include <assert.h>
#include <stdio.h>
#include <signal.h>

int mycb(struct event* evt, int fd, int mask, void* args)
{
    return 0;
}

int mytimer(struct event* evt, int fd, int mask, void* args)
{
    printf("timer value=%d\n", fd);

    if(fd == 1000){
        static int i = 0;
        if(i++ > 10){
            //evt_loop_exit(evt);
        }
    }
    return 0;
}

int mysignal(struct event* evt, int fd, int mask, void* args)
{
    printf("\nsignals=%d\n", fd);

    if(fd == SIGTERM){
        evt_loop_exit(evt);
    }

    return 0;
}

int main(int argc, char **argv)
{
    struct event* evt;

    int fd;

    //fd = tcp_noblock_clt("127.0.0.1", 95277);

    evt = evt_new();
    assert(evt != NULL);
    //evt_add(evt, fd, EVT_SOCK, EVT_RDWR, mycb, NULL);
 //   evt_add(evt, EVT_TIMER, 300, 0, mytimer, NULL);
    evt_add(evt, EVT_TIMER, 1000, 0, mytimer, NULL);
   // evt_add(evt, EVT_TIMER, 500, 0, mytimer, NULL);

    evt_add(evt, EVT_SIGNAL, SIGINT, 0, mysignal, NULL);
    evt_add(evt, EVT_SIGNAL, SIGTERM, 0, mysignal, NULL);

    evt_loop(evt);

    evt_del(evt);

    return 0;
}
