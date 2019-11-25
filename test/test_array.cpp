#include <iostream>
#include <gtest/gtest.h>
#include <vector>

#include <time.h>

extern "C"
{
#include "../src/array/CTL_array.h"
#include "../src/array/CTL_array.c"
}

using namespace std;

vector<int> stl_vector;
CTL_array ctl_array;

void test_new()
{
    srand((unsigned)time(NULL));
    int *array = new int[100];
    CTL_array_new(&ctl_array, array, array);
}

TEST(push, Test)
{
    for (size_t i = 0; i < 100; i++)
    {
        stl_vector.push_back(i);
        CTL_array_push_back(&ctl_array, i);
    }

    for (size_t i = 0; i < 100; i++)
    {
        ASSERT_TRUE(stl_vector[i] == ctl_array.begin.data[i]);
    }
}

TEST(erase, Test)
{
    for (size_t i = 0; i < 50; i++)
    {
        int pos = rand() % 50;
        stl_vector.erase(stl_vector.begin() + pos);
        auto at = CTL_array_at(&ctl_array, pos);
        CTL_array_erase(&ctl_array, &at);
    }

    for (size_t i = 0; i < 50; i++)
    {
        ASSERT_TRUE(stl_vector[i] == ctl_array.begin.data[i]);
    }
}

TEST(insert, Test)
{
    for (size_t i = 0; i < 50; i++)
    {
        int pos = rand() % 50;
        stl_vector.insert(stl_vector.begin() + pos, i);
        auto at = CTL_array_at(&ctl_array, pos);
        CTL_array_insert(&ctl_array, &at, i);
    }

    for (size_t i = 0; i < 100; i++)
    {
        ASSERT_TRUE(stl_vector[i] == ctl_array.begin.data[i]);
    }
}

TEST(iterator, move)
{
    auto stl_it = stl_vector.begin();
    auto ctl_it = ctl_array.begin;

    for (size_t i = 0; i < 100; i++)
    {
        ASSERT_TRUE(*stl_it == *ctl_it.data);
        ++stl_it;
        ctl_it = CTL_array_iterator_move(&ctl_it, 1, false);
    }

    for (size_t i = 0; i < 100; i++)
    {
        ASSERT_TRUE(*stl_it == *ctl_it.data);
        --stl_it;
        ctl_it = CTL_array_iterator_move(&ctl_it, 1, true);
    }
}

TEST(iterator, other)
{
    auto a = ctl_array.begin;
    auto b = ctl_array.end;
    ASSERT_FALSE(CTL_array_iterator_equal(&a, &b));
    ASSERT_FALSE(CTL_array_iterator_more(&a, &b));
    ASSERT_TRUE(CTL_array_iterator_more(&b, &a));
    ASSERT_TRUE(stl_vector.begin() - stl_vector.end() == CTL_array_iterator_diff(&a, &b));
    b = ctl_array.begin;
    ASSERT_TRUE(CTL_array_iterator_equal(&a, &b));
}

int main(int argc, char **argv)
{
    test_new();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}