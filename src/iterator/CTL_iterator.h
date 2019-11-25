#include <malloc.h>
#include <string.h>

//通用API
#define CTL_iterator_move(T_containers, handle, pos, sub) \
        T_containers##_move(handle, pos, sub);

#define CTL_iterator_equal(T_containers, left, right) \
        T_containers##_equal(left, right);

//仅限array vector deque
#define CTL_iterator_diff(T_containers, left, right) \
        T_containers##_diff(left, right);

#define CTL_iterator_more(T_containers, left, right) \
        T_containers##_more(left, right);