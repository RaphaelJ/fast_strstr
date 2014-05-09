#include <string.h>

char *naive_strstr(const char *haystack, const char *needle)
{
    const size_t needle_len = strlen(needle);

    while (*haystack != '\0') {
        if (strncmp(haystack, needle, needle_len) == 0)
            return (char *) haystack;
        else
            haystack++;
    }

    return NULL;
}
