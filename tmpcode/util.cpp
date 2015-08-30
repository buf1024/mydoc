/*
 * File   : util.cpp
 *
 * Purpose: 
 * Version: 2012-1-31 1.0 Created
 * Author : buf1024@gmail.com
 */
#include "util.h"
#include "errcode.h"

/*
 * Routine to see if a text string is matched by a wildcard pattern.
 * Returns true if the text is matched, or false if it is not matched
 * or if the pattern is invalid.
 *  *        matches zero or more characters
 *  ?        matches a single character
 *  [abc]    matches 'a', 'b' or 'c'
 *  \c       quotes character c
 *  Adapted from code written by Ingo Wilken.
 */
bool Match(const char * text, const char * pattern)
{
    const char * retryPat;
    const char * retryText;
    int ch;
    bool found;

    retryPat = NULL;
    retryText = NULL;

    while (*text || *pattern) {
        ch = *pattern++;

        switch (ch) {
        case '*':
            retryPat = pattern;
            retryText = text;
            break;

        case '[':
            found = false;

            while ((ch = *pattern++) != ']') {
                if (ch == '\\')
                    ch = *pattern++;

                if (ch == '\0')
                    return false;

                if (*text == ch)
                    found = true;
            }

            if (!found) {
                pattern = retryPat;
                text = ++retryText;
            }

            if (*text++ == '\0')
                return false;
            break;

        case '?':
            if (*text++ == '\0')
                return false;

            break;

        case '\\':
            ch = *pattern++;

            if (ch == '\0')
                return false;

            if (*text == ch) {
                if (*text)
                    text++;
                break;
            }

            if (*text) {
                pattern = retryPat;
                text = ++retryText;
                break;
            }

            return false;

        default:
            if (*text == ch) {
                if (*text)
                    text++;
                break;
            }

            if (*text) {
                pattern = retryPat;
                text = ++retryText;
                break;
            }

            return false;
        }

        if (pattern == NULL)
            return false;
    }

    return true;
}

int MakeDaemon()
{

    return E_SUCCESS;
}


