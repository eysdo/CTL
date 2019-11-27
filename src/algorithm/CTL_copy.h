#include <stdbool.h>

#define CTL_copy(Type_dest, Type_source, dest, source, n)              \
    do                                                                 \
    {                                                                  \
        for (size_t _i = 0; _i < n; ++_i)                              \
        {                                                              \
            *(dest).data = *(first).data;                              \
            (first) = Type_source##_iterator_move(&(first), 1, false); \
            (dest) = Type_dest##_iterator_move(&(dest), 1, false);     \
        }                                                              \
    } while (false);

#define CTL_move(Type_dest, Type_source, dest, source, n)                                   \
    do                                                                                      \
    {                                                                                       \
        Type_dest##_iterator dest_b = Type_dest##_iterator_move(&(dest), n, false);         \
        Type_source##_iterator source_b = Type_source##_iterator_move(&(source), n, false); \
        for (size_t _i = 0; _i <= n; ++_i)                                                  \
        {                                                                                   \
            *(dest_b).data = *(source_b).data;                                              \
            (dest_b) = Type_dest##_iterator_move(&(dest_b), 1, true);                       \
            (source_b) = Type_source##_iterator_move(&(source_b), 1, true);                 \
        }                                                                                   \
    } while (false);
