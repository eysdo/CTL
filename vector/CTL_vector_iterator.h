#include <stdint.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h>

typedef struct
{
    void **data;
    uint64_t pos;
} CTL_vector_iterator;

CTL_vector_iterator CTL_vector_iterator_add(CTL_vector_iterator handle, uint64_t len)
{
    handle.data = handle.data + len;
    handle.pos = handle.pos + len;
    return handle;
}

CTL_vector_iterator CTL_vector_iterator_sub(CTL_vector_iterator handle, uint64_t len)
{
    handle.data = handle.data - len;
    handle.pos = handle.pos - len;
    return handle;
}
/*

int CTL_vector_iterator_store(CTL_vector_iterator *handle, CTL_vector_iterator exp)
{
    *handle = exp;
    return 0;
}

bool CTL_vector_iterator_compare(CTL_vector_iterator handle, CTL_vector_iterator exp)
{
    if (handle.pos == exp.pos)
        return true;
    
    return false;
}

bool CTL_vector_iterator_less(CTL_vector_iterator handle, CTL_vector_iterator exp)
{
    if (handle.pos < exp.pos)
        return true;
    
    return false;
}

bool CTL_vector_iterator_more(CTL_vector_iterator handle, CTL_vector_iterator exp)
{
    if (handle.pos > exp.pos)
        return true;

    return false;
}
*/