#include <iostream>
#include <gtest/gtest.h>
#include <list>
#include <algorithm>
#include <time.h>

extern "C"
{
#include "../src/list/CTL_list.h"
#include "../src/list/CTL_list.c"
#include "../src/allocator/CTL_allocator.c"
}

using namespace std;

list<int> stl_list;
CTL_list ctl_list;

void test_new()
{
    srand((unsigned)time(NULL));
    CTL_list_new(&ctl_list);
}

TEST(push, Test)
{
    for (size_t i = 0; i < 100; i++)
    {
        stl_list.push_back(i);
        CTL_list_push_back(&ctl_list, i);
    }

    auto ctl_it = ctl_list.begin;
    auto stl_it = stl_list.begin();

    for (size_t i = 0; i < 100; i++)
    {
        ASSERT_TRUE(*stl_it == *ctl_it.data);
        ++stl_it;
        ctl_it = CTL_list_iterator_move(&ctl_it, 1, false);
    }
}

TEST(erase, Test)
{
    for (size_t i = 0; i < 50; i++)
    {
        int pos = rand() % 50;
        auto begin = stl_list.begin();
        for (size_t i = 0; i < pos; i++)
        {
            begin++;
        }

        stl_list.erase(begin);
        auto at = CTL_list_at(&ctl_list, pos);
        CTL_list_erase(&ctl_list, &at);
    }

    auto ctl_it = ctl_list.begin;
    auto stl_it = stl_list.begin();

    for (; stl_it != stl_list.end();)
    {
        ASSERT_TRUE(*stl_it == *ctl_it.data);
        ++stl_it;
        ctl_it = CTL_list_iterator_move(&ctl_it, 1, false);
    }
}

TEST(insert, Test)
{
    for (size_t i = 0; i < 50; i++)
    {
        int pos = rand() % 50;
        auto begin = stl_list.begin();
        for (size_t i = 0; i < pos; i++)
        {
            begin++;
        }
        auto at = CTL_list_at(&ctl_list, pos);
        CTL_list_insert(&ctl_list, &at, i);
        stl_list.insert(begin, i);
    }

    auto ctl_it = ctl_list.begin;
    auto stl_it = stl_list.begin();

    for (; stl_it != stl_list.end();)
    {
        ASSERT_TRUE(*stl_it == *ctl_it.data);
        ++stl_it;
        ctl_it = CTL_list_iterator_move(&ctl_it, 1, false);
    }
}

TEST(iterator, move)
{
    auto stl_it = stl_list.begin();
    auto ctl_it = ctl_list.begin;

    for (; stl_it != stl_list.end();)
    {
        ASSERT_TRUE(*stl_it == *ctl_it.data);
        ++stl_it;
        ctl_it = CTL_list_iterator_move(&ctl_it, 1, false);
    }

    for (size_t i = 0; i < 100; i++)
    {
        --stl_it;
        ctl_it = CTL_list_iterator_move(&ctl_it, 1, true);
        ASSERT_TRUE(*stl_it == *ctl_it.data);
    }
}

TEST(iterator, other)
{
    auto a = ctl_list.begin;
    auto b = ctl_list.end;
    ASSERT_FALSE(CTL_list_iterator_equal(&a, &b));
    b = ctl_list.begin;
    ASSERT_TRUE(CTL_list_iterator_equal(&a, &b));
}

TEST(splice, splice)
{
    list<int> stl_sp;
    CTL_list ctl_sp;
    CTL_list_new(&ctl_sp);
    for (size_t i = 0; i < 50; i++)
    {
        stl_sp.push_back(i);
        CTL_list_push_back(&ctl_sp, i);
    }
    stl_list.splice(stl_list.begin(), stl_sp, stl_sp.begin(), stl_sp.end());
    splice(&ctl_list, &ctl_list.begin, &ctl_sp, &ctl_sp.begin, &ctl_sp.end);

    auto ctl_it = ctl_list.begin;
    auto stl_it = stl_list.begin();
    for (; stl_it != stl_list.end();)
    {
        ASSERT_TRUE(*stl_it == *ctl_it.data);
        ++stl_it;
        ctl_it = CTL_list_iterator_move(&ctl_it, 1, false);
    }
    CTL_list_delete(&ctl_sp);
}

bool max(int a, int b)
{
    return a > b;
}

TEST(merge, merge)
{
    stl_list.clear();
    CTL_list_clear(&ctl_list);

    for (size_t i = 0; i < 100; i += 2)
    {
        stl_list.push_back(i);
        CTL_list_push_back(&ctl_list, i);
    }

    CTL_list ctl_list_2;
    CTL_list_new(&ctl_list_2);

    list<int> stl_list_2;
    for (size_t i = 0; i < 50; i += 2)
    {
        stl_list_2.push_back(i);
        CTL_list_push_back(&ctl_list_2, i);
    }

    stl_list.merge(stl_list_2);
    merge(&ctl_list, &ctl_list_2, max);

    auto ctl_it = ctl_list.begin;
    auto stl_it = stl_list.begin();
    
    for (; stl_it != stl_list.end();)
    {
        //cout<<*stl_it <<" " << *ctl_it.data <<endl;
        ASSERT_TRUE(*stl_it == *ctl_it.data);
        ++stl_it;
        ctl_it = CTL_list_iterator_move(&ctl_it, 1, false);
    }

   CTL_list_delete(&ctl_list_2);
}

TEST(allocator, delete)
{
    CTL_list_delete(&ctl_list);
    //cout<<CTL_debug_mem<<endl;
    ASSERT_TRUE(CTL_debug_mem == 0);
}

int main(int argc, char **argv)
{
    test_new();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}