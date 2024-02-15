#include <gtest/gtest.h>

#include <dbg/datastruct/RSQF.hpp>
#include <dbg/datastruct/quotienting.hpp>

#include <iostream>
using namespace std;

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

TEST(RSQF, insert_free_space)
{
  QuotientFilter<7, 5, LeftQuotienting> filter{};
  const LeftQuotienting quotienting{};

  // Prepare the example to insert
  const uint64_t quotient {2};
  const uint64_t rest {0b11011};
  const uint64_t val {(quotient << 5) | rest};

  // Perform insertion
  const auto qr {quotienting.compute<7, 5>(val)};
  filter.insert_in_free_space(qr);

  // Verify metadata
  ASSERT_EQ(filter.m_occupied.get(quotient), true);
  ASSERT_EQ(filter.m_runend.get(quotient), true);

  // Verify the rest value inserted
  const uint64_t block_idx {quotient / 64};
  ASSERT_EQ(filter.m_rests[block_idx].get(quotient % 64), rest);
}