#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/**
 * Finds the first occurrence of the sub-string needle in the string haystack.
 * Returns NULL if needle was not found.
 */
char *fast_strstr(const char *haystack, const char *needle)
{
    if (!*needle) // Empty needle.
        return (char *) haystack;

    // Runs strchr() on the first section of the haystack as it has a lower
    // algorithmic complexity for discarding the first non-matching characters.
    haystack = strchr(haystack, *needle);
    if (!haystack) // First character of needle is not in the haystack.
        return NULL;

    // First characters of haystack and needle are the same now. Both are
    // guaranteed to be at least one character long.
    // Now computes the sum of characters values of needle together with the
    // sum of the first needle_len characters of haystack.

    const char   *i_haystack    = haystack + 1
             ,   *i_needle      = needle   + 1;

    unsigned int  haystack_sum  = *haystack
               ,  needle_sum    = *haystack;
    bool          identical     = true;

    while (*i_haystack && *i_needle) {
        haystack_sum += *i_haystack;
        needle_sum   += *i_needle;
        identical    &= *i_haystack++ == *i_needle++;
    }

    // i_haystack now references the (needle_len + 1)-th character.

    if (*i_needle) // haystack is smaller than needle.
        return NULL;
    else if (identical)
        return (char *) haystack;

    size_t        needle_len = i_needle - needle;

    // Loops for the remaining of the haystack, updating the sum iteratively.
    const char   *prec_start;
    for (prec_start = haystack; *i_haystack; i_haystack++, prec_start++) {
        haystack_sum -= *prec_start;
        haystack_sum += *i_haystack;

        if (
               haystack_sum == needle_sum
            && memcmp(prec_start + 1, needle, needle_len) == 0
        )
            return (char *) prec_start + 1;
    }

    return NULL;
}
