/*
 * gmx_string.h
 *
 *  Created on: 2013-12-24
 *      Author: Luo Guochun
 */

#ifndef __GMX_STRING_H__
#define __GMX_STRING_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "gmx_core.h"
#include "gmx_queue.h"

typedef struct gmx_str_array_data_s
{
    char * d;
    gmx_slist_entry(gmx_str_array_data_s) entry;
}gmx_str_array_data_t;

typedef struct gmx_str_array_s
{
    size_t size;
    gmx_slist_head(gmx_str_arrary_head, gmx_str_array_data_s) list;

    char* (*at)(size_t index);

    int (*push)(const char* text, int size);
    char* (*pop)();
    int (*insert)(const char* text, int size);
    int (*delete)(const char* text, int size);

    size_t (*find)(const char* text, int size);
    size_t (*rfind)(const char* text, int size);

}gmx_str_array_t;

int gmx_init_str_array(gmx_str_array_t* arr);
int gmx_destroy_str_array(gmx_str_array_t* arr);

int gmx_get_hex(char ch);
int gmx_to_hex(const char* src, size_t src_len, char* dst, size_t dst_len);
int gmx_to_upper(const char* src, size_t src_len, char* dst, size_t dst_len);
int gmx_to_lower(const char* src, size_t src_len, char* dst, size_t dst_len);

int gmx_split(const char* text, char needle, gmx_str_array_t* dst);
int gmx_trim_left(const char* text, const char* needle, char* dst, size_t dst_len);
int gmx_trim_right(const char* text, const char* needle, char* dst, size_t dst_len);
int gmx_trim(const char* text, const char* needle, char* dst, size_t dst_len);

/*wildcard*/
int gmx_match(const char* text, const char* pattern);



#ifdef __cplusplus
}
#endif

#endif /* __GMX_STRING_H__ */
