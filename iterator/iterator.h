#include <malloc.h>
#include <string.h>

typedef int type;

#define iterator \
    type *data;  \
    ptrdiff_t pos;

typedef struct iterator_bidirectional
{
    type* data;
    ptrdiff_t pos;
}iterator_bidirectional;

test()
{
    memcpy
}