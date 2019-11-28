#include <stdbool.h>

#define CTL_copy(Type_dest, Type_source, dest, source, n)                    \
    do                                                                       \
    {                                                                        \
        for (size_t _i = 0; _i < (n); ++_i)                                  \
        {                                                                    \
            *(dest).data = *(first).data;                                    \
            (first) = CTL_##Type_source##_iterator_move(&(first), 1, false); \
            (dest) = CTL_##Type_dest##_iterator_move(&(dest), 1, false);     \
        }                                                                    \
    } while (false);

#define CTL_move(Type_dest, Type_source, dest, source, n)                     \
    do                                                                        \
    {                                                                         \
        dest = CTL_##Type_dest##_iterator_move(&(dest), n, false);            \
        source = CTL_##Type_source##_iterator_move(&(source), n, false);      \
        for (size_t _i = 0; _i <= (n); ++_i)                                  \
        {                                                                     \
            *(dest).data = *(source).data;                                    \
            (dest) = CTL_##Type_dest##_iterator_move(&(dest), 1, true);       \
            (source) = CTL_##Type_source##_iterator_move(&(source), 1, true); \
        }                                                                     \
    } while (false);

#define CTL_swap(Type, Type_a, Type_b, a, b, n)                            \
    do                                                                     \
    {                                                                      \
        for (size_t _i = 0; _i < (n); ++_i)                                \
        {                                                                  \
            Type _counter = *(a).data * (a).data = *(b).data;              \
            *(b).data = _counter;                                          \
            (a) = CTL_##Type_dest##_iterator_move(&(dest_b), 1, true);     \
            (b) = CTL_##Type_source##_iterator_move(&(source_b), 1, true); \
        }                                                                  \
                                                                           \
    } while (false);
