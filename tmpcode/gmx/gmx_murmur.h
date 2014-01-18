/*
 * gmx_murmur.h
 *
 *  Created on: 2013-12-24
 *      Author: Luo Guochun
 */

#ifndef __GMX_MURMUR_H__
#define __GMX_MURMUR_H__

#include "gmx_core.h"

uint32_t gmx_murmur_hash(uchar *data, size_t len);

#endif /* __GMX_MURMUR_H__ */
