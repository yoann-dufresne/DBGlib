#include <gtest/gtest.h>

#include <dbg/datastruct/RSQF.hpp>
#include <dbg/datastruct/quotienting.hpp>


static constexpr uint64_t q1 {6};
static constexpr uint64_t r1 {6};

// class QuotientFilter<q, r, LeftQuotienting>;

class QuotientFilter_6_6_Fixture : public QuotientFilter<q1, r1, LeftQuotienting>, public testing::Test
{};

static constexpr uint64_t q2 {8};
static constexpr uint64_t r2 {3};

class QuotientFilter_8_3_Fixture : public QuotientFilter<q2, r2, LeftQuotienting>, public testing::Test
{};


TEST_F(QuotientFilter_6_6_Fixture, init)
{
  ASSERT_EQ(std::size(m_rests), 1);
}

TEST_F(QuotientFilter_8_3_Fixture, init)
{
  ASSERT_EQ(std::size(m_rests), 4);
}
