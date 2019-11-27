#include <iostream>
#include <gtest/gtest.h>
#include <vector>

#include <time.h>

extern "C"
{
#include "../src/vector/CTL_vector.h"
#include "../src/vector/CTL_vector.c"
#include "../src/allocator/CTL_allocator.c"
}

using namespace std;

vector<int> stl_vector;
CTL_vector ctl_vector;

void test_new()
{
    srand((unsigned)time(NULL));
    CTL_vector_new(&ctl_vector, 10);
}

TEST(push, Test)
{
    for (size_t i = 0; i < 100; i++)
    {
        stl_vector.push_back(i);
        CTL_vector_push_back(&ctl_vector, i);
    }

    for (size_t i = 0; i < 100; i++)
    {
        ASSERT_TRUE(stl_vector[i] == ctl_vector.begin.data[i]);
    }
}

TEST(erase, Test)
{
    for (size_t i = 0; i < 50; i++)
    {
        int pos = rand() % 50;
        stl_vector.erase(stl_vector.begin() + pos);
        auto at = CTL_vector_at(&ctl_vector, pos);
        CTL_vector_erase(&ctl_vector, &at);
    }

    for (size_t i = 0; i < 50; i++)
    {
        ASSERT_TRUE(stl_vector[i] == ctl_vector.begin.data[i]);
    }
}

TEST(insert, Test)
{
    for (size_t i = 0; i < 50; i++)
    {
        int pos = rand() % 50;
        stl_vector.insert(stl_vector.begin() + pos, i);
        auto at = CTL_vector_at(&ctl_vector, pos);
        CTL_vector_insert(&ctl_vector, &at, i);
    }

    for (size_t i = 0; i < 100; i++)
    {
        ASSERT_TRUE(stl_vector[i] == ctl_vector.begin.data[i]);
    }
}

TEST(iterator, move)
{
    auto stl_it = stl_vector.begin();
    auto ctl_it = ctl_vector.begin;

    for (size_t i = 0; i < 100; i++)
    {
        ASSERT_TRUE(*stl_it == *ctl_it.data);
        ++stl_it;
        ctl_it = CTL_vector_iterator_move(&ctl_it, 1, false);
    }

    for (size_t i = 0; i < 100; i++)
    {
        ASSERT_TRUE(*stl_it == *ctl_it.data);
        --stl_it;
        ctl_it = CTL_vector_iterator_move(&ctl_it, 1, true);
    }
}

TEST(iterator, other)
{
    auto a = ctl_vector.begin;
    auto b = ctl_vector.end;
    ASSERT_FALSE(CTL_vector_iterator_equal(&a, &b));
    ASSERT_FALSE(CTL_vector_iterator_more(&a, &b));
    ASSERT_TRUE(CTL_vector_iterator_more(&b, &a));
    ASSERT_TRUE(stl_vector.begin() - stl_vector.end() == CTL_vector_iterator_diff(&a, &b));
    b = ctl_vector.begin;
    ASSERT_TRUE(CTL_vector_iterator_equal(&a, &b));
}

TEST(allocator, delete)
{
    CTL_vector_delete(&ctl_vector);
    ASSERT_TRUE(CTL_debug_mem == 0);
}

int main(int argc, char **argv)
{
    test_new();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}