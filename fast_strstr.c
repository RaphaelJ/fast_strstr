//
// This algorithm is licensed under the open-source BSD3 license
//
// Copyright (c) 2014, Raphael Javaux
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, 
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors
// may be used to endorse or promote products derived from this software without
// specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

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

    const char    needle_first  = *needle;

    // Runs strchr() on the first section of the haystack as it has a lower
    // algorithmic complexity for discarding the first non-matching characters.
    haystack = strchr(haystack, needle_first);
    if (!haystack) // First character of needle is not in the haystack.
        return NULL;

    // First characters of haystack and needle are the same now. Both are
    // guaranteed to be at least one character long.
    // Now computes the sum of the first needle_len characters of haystack
    // minus the sum of characters values of needle.

    const char   *i_haystack    = haystack + 1
             ,   *i_needle      = needle   + 1;

    unsigned int  sums_diff     = *haystack;
    bool          identical     = true;

    while (*i_haystack && *i_needle) {
        sums_diff += *i_haystack;
        sums_diff -= *i_needle;
        identical &= *i_haystack++ == *i_needle++;
    }

    // i_haystack now references the (needle_len + 1)-th character.

    if (*i_needle) // haystack is smaller than needle.
        return NULL;
    else if (identical)
        return (char *) haystack;

    size_t        needle_len    = i_needle - needle;
    size_t        needle_len_1  = needle_len - 1;

    // Loops for the remaining of the haystack, updating the sum iteratively.
    const char   *sub_start;
    for (sub_start = haystack; *i_haystack; i_haystack++) {
        sums_diff -= *sub_start++;
        sums_diff += *i_haystack;

        // Since the sum of the characters is already known to be equal at that
        // point, it is enough to check just needle_len-1 characters for
        // equality.
        if (
               sums_diff == 0
            && needle_first == *sub_start // Avoids some calls to memcmp.
            && memcmp(sub_start, needle, needle_len_1) == 0
        )
            return (char *) sub_start;
    }

    return NULL;
}
