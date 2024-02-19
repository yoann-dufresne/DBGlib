#include <gtest/gtest.h>

#include <dbg/datastruct/bitvector.hpp>


#include <iostream>
using namespace std;


TEST(bitvector, bitvector_init)
{
  Bitvector<128> vector{};
  ASSERT_EQ(vector.m_vector.size(), 2);

  Bitvector<129> vector_3uints{};
  ASSERT_EQ(vector_3uints.m_vector.size(), 3);
}

TEST(bitvector, bitvector_1bit_set)
{
  Bitvector<128> bv{};
  
  for (uint64_t i{0} ; i<128 ; i++)
  {
    bv.set(i);
    if (i<64)
    {
      ASSERT_EQ(bv.m_vector[0], 1UL << i);
      ASSERT_EQ(bv.m_vector[1], 0);
    }
    else
    {
      ASSERT_EQ(bv.m_vector[0], 0);
      ASSERT_EQ(bv.m_vector[1], 1UL << (i - 64));
    }
    bv.unset(i);
    ASSERT_EQ(bv.m_vector[0], 0);
    ASSERT_EQ(bv.m_vector[1], 0);
  }
}

TEST(bitvector, bitvector_unset_set_full)
{
  Bitvector<128> bv{};
  uint64_t expected {0};
  uint64_t max {~expected};
  
  // --- Set the bitvector bit by bit and test it ---

  for (uint64_t i{0} ; i<128 ; i++)
  {
    if (i == 64)
      expected = 0;
    expected <<= 1;
    expected |= 1;

    bv.set(i);
    if (i<64)
    {
      ASSERT_EQ(bv.m_vector[0], expected);
      ASSERT_EQ(bv.m_vector[1], 0);
    }
    else
    {
      ASSERT_EQ(bv.m_vector[0], max);
      ASSERT_EQ(bv.m_vector[1], expected);
    }
  }

  for (uint64_t i{0} ; i<128 ; i++)
  {
    if (i % 64 == 0)
      expected = max;
    expected <<= 1;

    bv.unset(i);
    if (i<64)
    {
      ASSERT_EQ(bv.m_vector[0], expected);
      ASSERT_EQ(bv.m_vector[1], max);
    }
    else
    {
      ASSERT_EQ(bv.m_vector[0], 0);
      ASSERT_EQ(bv.m_vector[1], expected);
    }
  }
}

TEST(bitvector, bitvector_get_1bit)
{
  Bitvector<128> bv{};
  uint64_t set_value;

  // --- Set the bitvector bit by bit and test it ---

  for (uint64_t i{0} ; i<128 ; i++)
  {
    // Compute the value to simulate the bitvector
    if (i % 64 == 0)
      set_value = 1;
    else
      set_value <<= 1;

    // Set the bitvector value by replacing 64 bits chuncks
    if (i < 64)
    {
      bv.m_vector[0] = set_value;
      bv.m_vector[1] = 0;
    }
    else
    {
      bv.m_vector[0] = 0;
      bv.m_vector[1] = set_value;
    }

    // Test all the 128 values
    for (uint64_t j{0} ; j<128 ; j++)
    {
      ASSERT_EQ(bv.get(j), i==j);
    }
  }
}

TEST(bitvector, single_uint_shift)
{
  // 3 uints bitvector
  Bitvector<192> bv{};
  // Init
  for (uint64_t i{3} ; i<=6 ; i++) bv.set(i);

  // Large shift : Everythng should be shifted of 1 position
  bv.toric_right_shift(0, 10);
  ASSERT_EQ(bv.m_vector[0], 0b11110000UL);
  ASSERT_EQ(bv.m_vector[1], 0);
  ASSERT_EQ(bv.m_vector[2], 0);

  // Reinit
  bv.clear();
  for (uint64_t i{3} ; i<=6 ; i++) bv.set(i);

  // shift 1s : 1 bit should be lost by overflowing
  bv.toric_right_shift(3, 6);
  ASSERT_EQ(bv.m_vector[0], 0b01110000UL);
  ASSERT_EQ(bv.m_vector[1], 0);
  ASSERT_EQ(bv.m_vector[2], 0);

  // Reinit
  bv.clear();
  for (uint64_t i{3} ; i<=6 ; i++) bv.set(i);

  // Truncated shift : The less significant bit of the slice should be set to 0 after shift
  bv.toric_right_shift(5, 10);
  ASSERT_EQ(bv.m_vector[0], 0b11011000UL);
  ASSERT_EQ(bv.m_vector[1], 0);
  ASSERT_EQ(bv.m_vector[2], 0);
}
