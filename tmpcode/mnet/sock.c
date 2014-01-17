/*
 * sock.c
 *
 *  Created on: 2012-12-14
 *      Author: buf1024@gmail.com
 */

#include "sock.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <string.h>

#define EVT_SOCK_BACKLOG 128

int tcp_svr(const char* addr, int port)
{
    int fd = -1;
    int flag = 0;
    struct sockaddr_in saddr;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
        return -1;

    flag = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) != 0)
        return -1;

    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY );
    if (addr) {
        if (inet_aton(addr, &saddr.sin_addr) == 0)
            return -1;
    }
    saddr.sin_port = htons(port);
    if (bind(fd, (struct sockaddr*) (&saddr), sizeof(saddr)) < 0)
        return -1;

    if (listen(fd, EVT_SOCK_BACKLOG) < 0)
        return -1;

    return 0;
}
int tcp_connect(const char* addr, int port, int flag)
{
    int fd;
    struct sockaddr_in sa;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
        return -1;

    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    if (inet_aton(addr, &sa.sin_addr) == 0) {
        struct hostent *he;

        he = gethostbyname(addr);
        if (he == NULL )
            return -1;
        memcpy(&sa.sin_addr, he->h_addr, sizeof(struct in_addr));
    }
    if (connect(fd, (struct sockaddr*) &sa, sizeof(sa)) == -1) {
        if (errno == EINPROGRESS && flag)
            return fd;

        close(fd);
        return -1;
    }
    return fd;
}
int set_opt(int fd, int opt)
{
    int flags;

    flags = fcntl(fd, F_GETFL);
    if (flags == -1)
        return -1;
    flags |= opt;
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
        return -1;
    return 0;
}

int tcp_accept(int fd, char* ip, int* port)
{
    struct sockaddr_in sa;
    socklen_t len = sizeof(sa);
    int cltfd;
    while (1) {
        cltfd = accept(fd, (struct sockaddr*)&sa, &len);
        if (cltfd == -1) {
            if (errno == EINTR)
                continue;
            else {
                return -1;
            }
        }
        break;
    }
    if (ip)
        strcpy(ip, inet_ntoa(sa.sin_addr));
    if (port)
        *port = ntohs(sa.sin_port);
    return cltfd;
}

