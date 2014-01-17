/*
 * sock.h
 *
 *  Created on: 2012-12-14
 *      Author: buf1024@gmail.com
 */

#ifndef __48SLOTS_SOCK_H__
#define __48SLOTS_SOCK_H__

int tcp_svr(const char* addr, int port);
int tcp_connect(const char* addr, int port, int flag);

int set_opt(int fd, int opt);

int tcp_accept(int fd, char* ip, int* port);

#endif /* __48SLOTS_SOCK_H__ */
