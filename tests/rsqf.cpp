#include <gtest/gtest.h>

#include <dbg/datastruct/RSQF.hpp>
#include <dbg/datastruct/quotienting.hpp>


#include <iostream>
using namespace std;


TEST(rsqf_monoblock, init)
{
  QuotientFilter<6, 6, left_quotienting<6, 6> > rsqf {};
  
  ASSERT_EQ(std::size(rsqf.rests), 1);
}
