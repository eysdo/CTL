#include "stdbool.h"

#define CTL_copy(Type_destin , Type_source, destin, source, n) \
    do                                      \
    {                                       \
        Type_destin current = (destin)     \
            Type_source next;   \
        for (size_t i = 0; i < n; ++i)
        {                                   \
            Type_iterator_##add(&next, 1);  \
            *(iterator).data = *next.data;  \
        }                                   \
    } while (false);
