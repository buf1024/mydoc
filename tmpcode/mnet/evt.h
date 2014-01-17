/*
 * evt.h
 *
 *  Created on: 2012-12-14
 *      Author: buf1024@gmail.com
 */

#ifndef __48SLOTS_EVT_H__
#define __48SLOTS_EVT_H__

#include <stdlib.h> //size_t
#include "list.h"

//not validate the arguments.
//invoker must respond for passing the correct arguments

#ifdef __cplusplus
extern "C" {
#endif

#define EVT_MAX_EVENT             1024*10
#define EVT_DEFAULT_POLL_TIMEOUT  100

struct event;
typedef int (*evt_callback)(struct event* evt, int fd, int mask, void* args);

struct event
{
    int exit;
    int defto;                // if poll time out not specific use this as the poll timeout value
    struct evt_timeout* to;   // poll timeout list
    struct list* fds;         //
    struct list* signals;     //
    struct list* timers;      //
    void* data;               // back-end specific data
};
struct evt_poll
{
    int fd;               //monitor fd
    int mask;             //interest event
    int evt;              //event fired
    evt_callback cb;
    void* args;
    struct evt_buf* buf;  //buffer
};
struct evt_signal
{
    int sig;             //interest signo
    evt_callback cb;     //calback function
    void* args;          //callback args
};
struct evt_timer
{
    int intval;     //in ms
    unsigned long lastval;    //last time
    evt_callback cb;
    void* args;
};
struct evt_timeout
{
    int flag;
    int timeout;    //is ms
    evt_callback cb;
    void* args;
};

//no thread safe
struct evt_buf
{
    size_t cap;
    char*  rbuf;
    size_t rpos;
    size_t rwater;

    char*  wbuf;
    size_t wpos;
    size_t wwater;
};

enum evt_mask
{
    EVT_READ  = 0x01,
    EVT_WRITE = 0x02,
    EVT_RDWR  = 0x03, // 0x02 & 0x01 = 0x03
    EVT_HUP   = 0x04,
    EVT_ERROR = 0x08
};

enum evt_type
{
    EVT_SOCK      = 0x01,
    EVT_SIGNAL    = 0x02,
    EVT_TIMER     = 0x04,
    EVT_TIMEOUT   = 0x08
};

struct evt_buf* evt_buf_new();
int evt_buf_del(struct evt_buf* buf);

int evt_buf_set_attr(struct evt_buf* buf, size_t cap, size_t rwater, size_t wwater);
int evt_buf_rget(struct evt_buf* buf, char* req, size_t s);
int evt_buf_rdrag(struct evt_buf* buf, char* req, size_t s);
int evt_buf_rput(struct evt_buf* buf, const char* res, size_t s);
int evt_buf_wget(struct evt_buf* buf, char* req, size_t s);
int evt_buf_wdrag(struct evt_buf* buf, char* req, size_t s);
int evt_buf_wput(struct evt_buf* buf, const char* res, size_t s);

#define evt_buf_free_rsize(buf) do{buf->cap - buf->rpos}while(0)
#define evt_buf_free_wsize(buf) do{buf->cap - buf->wpos}while(0)
#define evt_buf_roverflow(buf) (buf->rpos >= buf->rwater)
#define evt_buf_woverflow(buf) (buf->wpos >= buf->wwater)

struct evt_buf* evt_get_buf(struct event* evt, int fd);
int evt_assoc_buf(struct event* evt, int fd, struct evt_buf* buf);

struct event* evt_new();
int evt_del(struct event* evt);

// if type == EVT_SOCK, fd = fd, mask = interest event
// if type == EVT_SIGNAL fd = signo, mask not use
// if type == EVT_TIMER fd = interval, mask not use
// if type == EVT_TIMEOUT fd = poll timeout value, mask not use
int evt_add(struct event* evt, int type, int fd, int mask, evt_callback cb, void* args);
int evt_drop(struct event* evt, int fd, int type);

int evt_loop(struct event* evt);
int evt_loop_exit(struct event* evt);

#ifdef __cplusplus
}
#endif

#endif /* __48SLOTS_EVT_H__ */
