#include <gtest/gtest.h>

#include <dbg/datastruct/RSQF.hpp>
#include <dbg/datastruct/quotienting.hpp>


static constexpr uint64_t q {6};
static constexpr uint64_t r {6};

// class QuotientFilter<q, r, LeftQuotienting>;

class QuotientFilterFixture : public QuotientFilter<q, r, LeftQuotienting>, public testing::Test
{

};


TEST_F(QuotientFilterFixture, init)
{
  ASSERT_EQ(std::size(rests), 1);
}
