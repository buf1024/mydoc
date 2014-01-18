/*
 * gmx_base64.h
 *
 *  Created on: 2013-12-24
 *      Author: Luo Guochun
 */

#ifndef __GMX_BASE64_H__
#define __GMX_BASE64_H__

#include "gmx_core.h"

#define GMX_BASE64_ENCODED_LEN(len)  (((len + 2) / 3) * 4)
#define GMX_BASE64_DECODED_LEN(len)  (((len + 3) / 4) * 3)

#endif /* __GMX_BASE64_H__ */
