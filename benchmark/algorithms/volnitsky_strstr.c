#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *volnitsky_strstr(const char *haystack, const char *needle)
{
    // Only for little-endian platforms and where access to misaligned W is
    // allowed.

    typedef         uint16_t        W_t;
    typedef         uint8_t         offset_t;

    const size_t W_size         = sizeof(W_t)
               , haystack_len   = strlen(haystack)
               , needle_len     = strlen(needle)
               , step           = needle_len - W_size + 1;

    int          i;
    size_t       h;

    if (needle_len < 2*W_size-1 || needle_len >= UINT8_MAX)
        return strstr(haystack, needle); // fall-back to stdlib search.

    // Make the hash table
    const size_t H_size         = 65536;
    offset_t     H[65536]       = {0};     // initializes ALL buckets with 0

    for (i = needle_len - W_size; i >= 0; i--) {
        h = *((W_t *) (needle + i)) % H_size;

        while (H[h])
            h = (h+1) % H_size; // find free cell

        H[h] = i+1;
    }

    // step through text
    const char *P       = haystack + needle_len   - W_size
             , *P_end   = haystack + haystack_len - needle_len;
    for (; P <= P_end; P += step) {
        for (h = *((W_t*) P) % H_size; H[h]; h = (h+1) % H_size) {
            const char* R = P - (H[h] - 1);

            for (i = 0; i < needle_len; i++) {
                if (R[i] != needle[i])
                    goto next_hash_cell;
            }
            return (char *) R; // found

            next_hash_cell:;
        }
    }

    // check tail
    return strstr(P - step + 1, needle);
}

// int main(int argc, char **argv)
// {
//     fprintf(stderr, "%s %s\n", argv[1], argv[2]);
// 
//     char *res = fast_strstr(argv[1], argv[2]);
// 
//     if (res == NULL)
//         printf("Not found\n");
//     else
//         printf("%ld : '%s'\n", res - argv[1], res);
// }
