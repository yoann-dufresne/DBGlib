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

TEST(RSQF, insert_and_shift)
{
    QuotientFilter<7, 5, LeftQuotienting> filter{};
    const LeftQuotienting quotienting{};

    const uint64_t quotient {2};
    const uint64_t rest1 {0b11011};
    const uint64_t val1 {(quotient << 5) | rest1};

    const auto qr {quotienting.compute<7, 5>(val1)};
    filter.insert_in_free_space(qr);

    // --- Insert at the beginning of a run ---
    const uint64_t rest2 {0b01000};
    const auto val2 {quotienting.compute<7, 5>((quotient << 5) | rest2)};
    // const uint64_t val2 {(quotient << 5) | rest2};
    filter.insert_and_shift(val2, quotient, quotient+1);

    // Verification of the run start
    ASSERT_EQ(filter.m_occupied.get(quotient), true);
    ASSERT_EQ(filter.m_occupied.get(quotient+1), false);
    // Verification of the run end
    ASSERT_EQ(filter.m_runend.get(quotient), false);
    ASSERT_EQ(filter.m_runend.get(quotient+1), true);
    // Are the rests at the right place ?
    const uint64_t block_idx {quotient / 64};
    ASSERT_EQ(filter.m_rests[block_idx].get(quotient % 64), rest2);
    ASSERT_EQ(filter.m_rests[block_idx].get((quotient + 1) % 64), rest1);

    // --- Insert at the beginning of a run and shift 2 values ---
    const uint64_t rest3 {0b00001};
    const auto val3 {quotienting.compute<7, 5>((quotient << 5) | rest3)};
    // const uint64_t val2 {(quotient << 5) | rest2};
    filter.insert_and_shift(val3, quotient, quotient+2);

    // Verification of the run start
    ASSERT_EQ(filter.m_occupied.get(quotient), true);
    // Verification of the run end
    ASSERT_EQ(filter.m_runend.get(quotient+1), false);
    ASSERT_EQ(filter.m_runend.get(quotient+2), true);
    // Are the rests at the right place ?
    ASSERT_EQ(filter.m_rests[block_idx].get(quotient % 64), rest3);
    ASSERT_EQ(filter.m_rests[block_idx].get((quotient + 1) % 64), rest2);
    ASSERT_EQ(filter.m_rests[block_idx].get((quotient + 2) % 64), rest1);

    // --- Insert at the end of a run ---
    const uint64_t rest4 {0b11111};
    const auto val4 {quotienting.compute<7, 5>((quotient << 5) | rest4)};
    // const uint64_t val2 {(quotient << 5) | rest2};
    filter.insert_and_shift(val4, quotient+3, quotient+3);

    // Verification of the run end
    ASSERT_EQ(filter.m_runend.get(quotient+2), false);
    ASSERT_EQ(filter.m_runend.get(quotient+3), true);
    // Are the rests at the right place ?
    ASSERT_EQ(filter.m_rests[block_idx].get((quotient+3) % 64), rest4);
}


TEST(RSQF_first_unused, in_block)
{
    QuotientFilter<7, 5, LeftQuotienting> filter{};
    const LeftQuotienting quotienting{};
}
