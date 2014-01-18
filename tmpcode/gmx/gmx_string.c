/*
 * gmx_string.c
 *
 *  Created on: 2013-12-24
 *      Author: Luo Guochun
 */

#include "gmx_string.h"

static char* __gmx_str_arr_at(size_t index)
{
    return NULL ;
}

static int __gmx_str_arr_push(const char* text)
{
    return 0;
}
static char* __gmx_str_arr_pop()
{
    return NULL ;
}
static int __gmx_str_arr_insert(const char* text)
{
    return 0;
}
static int __gmx_str_arr_delete(const char* text)
{
    return 0;
}

static size_t __gmx_str_arr_find(const char* text)
{
    return 0;
}
static size_t __gmx_str_arr_rfind(const char* text)
{
    return 0;
}

int gmx_init_str_array(gmx_str_array_t* arr)
{
    return 0;
}
int gmx_destroy_str_array(gmx_str_array_t* arr)
{
    return 0;
}

int gmx_get_hex(char ch)
{
    ch = (0x0f & ch);
    if (ch >= 10) {
        return 'a' + ch - 10;
    }
    return '0' + ch;
}

int gmx_to_hex(const char* src, size_t src_len, char* dst, size_t dst_len)
{
    if (dst_len < (src_len + src_len)) {
        return -1; // buffer too small
    }
    for (int i = 0; i < src_len; i++) {
        char ch = *(src + i);
        dst[2 * i] = gmx_get_hex((char) ((0xf0 & ch) >> 4));
        dst[2 * i + 1] = gmx_get_hex((char) (0x0f & ch));

    }
    return 0;
}
int gmx_to_upper(const char* src, size_t src_len, char* dst, size_t dst_len)
{
    if (dst_len < src_len) {
        return -1; // buffer too small
    }
    while (src_len) {
        *dst = toupper(*src);
        dst++;
        src++;
        src_len--;
    }
    return 0;
}
int gmx_to_lower(const char* src, size_t src_len, char* dst, size_t dst_len)
{
    if (dst_len < src_len) {
        return -1; // buffer too small
    }
    while (src_len) {
        *dst = tolower(*src);
        dst++;
        src++;
        src_len--;
    }
    return 0;
}

int gmx_split(const char* text, char needle, gmx_str_array_t* dst)
{
    if (NULL == text || *text == 0 || dst == NULL) {
        return -1;
    }
    const char* orig = text;
    const char* tmp = text;
    while ((tmp = strchr(tmp, needle)) != NULL) {
        int len = tmp - orig;

        dst->push(orig, len);

        tmp++;

        orig = tmp;
    }
    if(orig != NULL){
        dst->push(orig, -1);
    }
    if(orig == text){
        dst->push(text, -1);
    }
    return 0;
}
int gmx_trim_left(const char* text, const char* needle, char* dst, size_t dst_len)
{
    const char* p = text;
    while (*p != 0) {
        if (NULL != strchr(needle, *p)) {
            p++;
        } else {
            break;
        }
    }

    int size = p - text;
    if (p != text) {
        if(size < dst_len) {
            return -1;
        }
        if(size > 0) {
            memcpy(dst, p, size);
            *(dst + size) = 0;
        }else{
            *dst = 0;
        }
    }

    return size;
}
int gmx_trim_right(const char* text, const char* needle, char* dst, size_t dst_len)
{
    int len = strlen(text);
    char *p = text + len - 1;
    while (p >= text) {
        if (strchr(needle, *p) != NULL) {
            --p;
        } else {
            break;
        }

    }
    int size = p - text;
    if (p != text + len - 1) {
        if(size < dst_len) {
            return -1;
        }
        if(size > 0) {
            memcpy(dst, text, size);
            *(dst + size) = 0;
        }else{
            *dst = 0;
        }
    }

    return size;
}
int gmx_trim(const char* text, const char* needle, char* dst, size_t dst_len)
{
    int len = gmx_trim_left(text, needle, dst, dst_len);
    if(len > 0) {
        len = gmx_trim_right(text + len, needle, dst, dst_len);
    }
    return len;
}
