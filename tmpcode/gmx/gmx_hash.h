/*
 * gmx_hash.h
 *
 *  Created on: 2013-12-24
 *      Author: Luo Guochun
 */

#ifndef __GMX_HASH_H__
#define __GMX_HASH_H__

/*using openssl*/
/*
#include "gmx_md5.h"
#include "gmx_sha1.h"
#include "gmx_mac.h"
#include "gmx_crc32.h"
*/

#ifdef __cplusplus
extern "C" {
#endif

#define GMX_MD5_LEN       32
#define GMX_SHA1_LEN      40
#define GMX_HMAC_MD5_LEN  32
#define GMX_HMAC_SHA1_LEN 40

int gmx_md5(void* data, int len, char* dst, int dst_len);
int gmx_md5_file(char* file, char* dst, int dst_len);

int gmx_sha1(void* data, int len, char* dst, int dst_len);
int gmx_sha1_file(char* file, char* dst, int dst_len);

int gmx_hmac_md5(void* data, int len, char* key,
        int key_len, char* dst, int dst_len);
int gmx_hmac_sha1(void* data, int len, char* key,
        int key_len, char* dst, int dst_len);

#ifdef __cplusplus
}
#endif
#endif /* __GMX_HASH_H__ */
