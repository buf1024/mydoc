/*
 * gmx_core.h
 *
 *  Created on: 2013-12-24
 *      Author: Luo Guochun
 */

#ifndef __GMX_CORE_H__
#define __GMX_CORE_H__

#if defined(__WIN32__) || defined(__WIN64__)
#include "gmx_os_win.h"
#endif

#if defined(__linux__) || defined(__unix__)
#include "gmx_os_linux.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>

typedef unsigned char uchar;

#define LF     (u_char) 10
#define CR     (u_char) 13
#define CRLF   "\x0d\x0a"

#define gmx_abs(value)       (((value) >= 0) ? (value) : - (value))
#define gmx_max(val1, val2)  ((val1 < val2) ? (val2) : (val1))
#define gmx_min(val1, val2)  ((val1 > val2) ? (val2) : (val1))



#endif /* __GMX_CORE_H__ */
