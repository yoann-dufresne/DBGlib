#include <gtest/gtest.h>
#include <dbg/datastruct/PackedArray.hpp>

// #include <iostream>
// using namespace std;


TEST(packedblock, init)
{
    PackedBlock<33> block {};

    // EXPECT_EQ(block.get(2), 0) << "block not initialized with 0 at index " << 2;
    for (size_t i=0 ; i<64 ; i++)
    {
        ASSERT_EQ(block.get(i), 0) << "block not initialized with 0 at index " << i;
    }
}

TEST(packedblock, insert_1s)
{
    PackedBlock<33> block {};
    const uint64_t ones { ( 1UL << 33 ) - 1};

    // EXPECT_EQ(block.get(2), 0) << "block not initialized with 0 at index " << 2;
    for (size_t i=0 ; i<64 ; i++)
    {
        block.set(i, ones);

        for (size_t j=0 ; j<64 ; j++)
        {
            if (i == j)
                ASSERT_EQ(block.get(j), ones) << "1s not set at index " << j;
            else
                ASSERT_EQ(block.get(j), 0) << "unexpected non-0 at index " << j;
        }

        block.set(i, 0);
        ASSERT_EQ(block.get(i), 0) << "unexpected non-0 at index " << i;
    }
}


TEST(packedblock, insert_in_run)
{
    PackedBlock<33> block {};
    const uint64_t ones { ( 1UL << 33 ) - 1};
    const uint64_t borders { (ones >> 1) - 1};

    // EXPECT_EQ(block.get(2), 0) << "block not initialized with 0 at index " << 2;
    for (size_t i=0 ; i<64 ; i++)
    {
        // Prepare a run
        if (i != 0)
            block.set(i-1, borders);
        if (i != 63)
            block.set(i+1, borders);

        // Set the value
        block.set(i, ones);

        for (size_t j=0 ; j<64 ; j++)
        {
            if (i == j)
                ASSERT_EQ(block.get(j), ones) << "1s not set at index " << j;
            else if (j == i-1 or j == i+1)
                ASSERT_EQ(block.get(j), borders) << "Nearby values modified at index " << j;
            else
                ASSERT_EQ(block.get(j), 0) << "unexpected non-0 at index " << j;
        }

        // Clean the run
        if (i != 0)
            block.set(i-1, 0);
        if (i != 63)
            block.set(i+1, 0);
        block.set(i, 0);
    }
}