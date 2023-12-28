#include <gtest/gtest.h>

#include <dbg/datastruct/RSQF.hpp>
#include <dbg/datastruct/quotienting.hpp>


#include <iostream>
using namespace std;

// template<uint64_t q, uint64_t r, typename Quotienting>
// class rsqf_test : QuotientFilter
// {
// public:
//   rsqf_test();
//   ~rsqf_test();
  
// };


TEST(rsqf_monoblock, init)
{
  constexpr uint64_t q {6};
  constexpr uint64_t r {6};

  QuotientFilter<q, r, LeftQuotienting> rsqf {};
  
  // ASSERT_EQ(std::size(rsqf.rests), 1);
}
