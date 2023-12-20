#include <gtest/gtest.h>
#include <dbg/datastruct/QF.hpp>


TEST(qf_rests, qf_rests_init)
{
  RestBlock<33> rests {};

  for (std::size_t i = 0; i < 64; ++i)
  {
    EXPECT_EQ(rests.get(i), 0) << "rests not initialized with 0 at index " << i;
  }
}