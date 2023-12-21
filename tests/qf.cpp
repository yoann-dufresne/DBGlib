#include <gtest/gtest.h>
#include <dbg/datastruct/PackedArray.hpp>


TEST(packedblock, packedblock_init)
{
  PackedBlock<33> block {};

  // EXPECT_EQ(block.get(2), 0) << "block not initialized with 0 at index " << 2;
  for (size_t i=0 ; i<64 ; i++)
  {
    ASSERT_EQ(block.get(i), 0) << "block not initialized with 0 at index " << i;
  }
}

TEST(packedblock, packedblock_insert_1s)
{
  PackedBlock<33> block {};
  uint64_t ones = ( 1UL << 34 ) - 1;

  // EXPECT_EQ(block.get(2), 0) << "block not initialized with 0 at index " << 2;
  for (size_t i=0 ; i<64 ; i++)
  {
    block.set(i, ones);

    for (size_t j=0 ; j<64 ; j++)
    {
      if (i == j)
        ASSERT_EQ(block.get(i), ones) << "1s not set at index " << i;
      else
        EXPECT_EQ(block.get(i), 0) << "unexpected non-0 at index " << i;
    }

    block.set(i, 0);
  }
}