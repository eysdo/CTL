#include <iostream>
#include <gtest/gtest.h>
#include <list>

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
    //    cout<<*stl_list.end()<<*ctl_list.end.data<<endl;
    //    cout<<*stl_it<<*ctl_it.data<<endl;
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

    for (;stl_it != stl_list.end();)
    {
        //cout<<*stl_it<<*ctl_it.data<<endl;
        //cout<<*stl_list.end()<<ctl_list.begin.node->next->data<<endl;
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
        //cout<<*stl_it<<*ctl_it.data<<endl;
        //cout<<*stl_list.end()<<ctl_list.begin.node->next->data<<endl;
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

TEST(allocator, delete)
{
    CTL_list_delete(&ctl_list);
    ASSERT_TRUE(CTL_debug_mem == 0);
}

int main(int argc, char **argv)
{
    test_new();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}