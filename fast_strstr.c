#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * Finds the first occurrence of the substring needle in the string haystack.
 * Returns NULL if needle was not found.
 */
char *fast_strstr(const char *haystack, const char *needle)
{
    int          i;

    // Computes the length of needle together with the sum of its characters.
    const char   *i_needle;
    unsigned int  needle_sum    = 0;
    for (i_needle = needle; *i_needle; i_needle++)
        needle_sum += *i_needle;

    size_t        needle_len = i_needle - needle;

    // Computes the sum of the first needle_len characters of the haystack.
    unsigned int  cur_sum       = 0;
    const char   *i_haystack    = haystack;
    for (i = 0; i < needle_len; i++, i_haystack++) {
        if (!*i_haystack) // Needle is longer than haystack.
            return NULL;

        cur_sum += *i_haystack;
    }

    // i_haystack now references the (needle_len + 1)-th character.

    if (cur_sum == needle_sum) {
        if (memcmp(haystack, needle, needle_len) == 0)
            return (char *) haystack;
    }

    // Loops for the remaining of the haystack, updating the sum iteratively.
    const char   *prec_start;
    for (prec_start = haystack; *i_haystack; i_haystack++, prec_start++) {
        cur_sum -= *prec_start;
        cur_sum += *i_haystack;

        if (
               cur_sum == needle_sum
            && memcmp(prec_start + 1, needle, needle_len) == 0
        )
            return (char *) prec_start + 1;
    }

    return NULL;
}
