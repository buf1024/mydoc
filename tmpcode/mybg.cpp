/*
 * mybg.cpp
 *
 *  Created on: 2013-2-1
 *      Author: Luo Guochun
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

#define REGISTER_SIGNAL(iSigType, pFunc, bInterrupt)\
    if(set_signal(iSigType, pFunc, bInterrupt) == SIG_ERR)\
    {\
        printf("register signal %d failed:%s\n", iSigType, strerror(errno));\
        return -1;\
    }

typedef void sigfunc(int);
sigfunc* set_signal(int iSigType, sigfunc* pFunc, bool bInterrupt);
void daemonlize();
int reg_sign();
void sig_handle(int iSigno);
void handle_signal();
void usage();
int service();

std::string  g_prog_name;
std::string  g_run_name;
bool g_exit = false;

//��ӡʹ�÷���
void usage()
{
    printf("Usage : %s [p:h][-p program][-h]\n", g_prog_name.c_str());
    printf("        -p input program file\n");
    printf("        -h show help\n");
}

//�ź�����
sigfunc* set_signal(int iSigType, sigfunc* pFunc, bool bInterrupt)
{
    struct sigaction act, oact;
    act.sa_handler = pFunc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    if (bInterrupt == false) //���жϵ���
            {
        act.sa_flags |= SA_RESTART;
    }
    if (sigaction(iSigType, &act, &oact) < 0) {
        return SIG_ERR ;
    }
    return oact.sa_handler;
}
void daemonlize()
{
    int pid = 0;
    if((pid = fork()) > 0)
    {
        exit(0);
    }
    setsid();
    if((pid = fork()) > 0)
    {
        exit(0);
    }
}

//ע���ź�
int reg_sign()
{
    REGISTER_SIGNAL(SIGTERM, sig_handle, true);
    //Kill�ź�
    REGISTER_SIGNAL(SIGINT, sig_handle, true);
    //�ն�CTRL-C�ź�
    REGISTER_SIGNAL(SIGUSR2, SIG_IGN, false);
    //����SIGUSR2�ź�
    REGISTER_SIGNAL(SIGHUP, SIG_IGN, false);
    //����SIGHUP�ź�
    REGISTER_SIGNAL(SIGUSR1, SIG_IGN, false);
    //����SIGUSR1�ź�
    REGISTER_SIGNAL(SIGCHLD, SIG_IGN, false);
    //�����ӽ����˳�
    //��һ���Ѿ��رյ�SOCKET send���ݣ��ں˻��������һ��PIPE�ź�
    REGISTER_SIGNAL(SIGPIPE, SIG_IGN, false);
    //����SIGPIPE�ź�
    REGISTER_SIGNAL(SIGALRM, SIG_IGN, false);
    //����SIGPIPE�ź�
    return 0;
}

//��ֹ�źŴ���
void sig_handle(int iSigno)
{
    if(iSigno == SIGTERM || iSigno == SIGINT){
        g_exit = true;
    }
}
void handle_signal()
{
}

int main(int argc, char* argv[])
{
    extern char *optarg;
    int optch;
    char optstring[] = "p:h";

    g_prog_name = argv[0];

    //��ȡ�����в���
    while ((optch = getopt(argc, argv, optstring)) != -1) {
        switch (optch) {
        case 'h':
            usage();
            exit(0);
        case 'p':
            g_run_name = optarg;
            break;
        default:
            usage();
            exit(0);
        }
    }

    if (g_run_name.empty()) {
        printf("program file is empty.\n");
        usage();
        exit(0);
    }

    daemonlize();

    srand(time(0));

    service();

    return 0;
}
int service()
{
    int ret = 0;
    int t = 0;
    while(!g_exit){
        ret = system(g_run_name.c_str());
        if(ret == -1){
            printf("fail to execute: %s.\n", g_run_name.c_str());
            break;
        }
        t = rand() % 1000;
        usleep(t);
    }
    return 0;
}
