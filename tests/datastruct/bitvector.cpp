#include <gtest/gtest.h>

#include <dbg/datastruct/bitvector.hpp>


#include <iostream>
#include <bit>
using namespace std;


TEST(bitvector, init)
{
    Bitvector<128> vector{};
    ASSERT_EQ(vector.m_vector.size(), 2);

    Bitvector<129> vector_3uints{};
    ASSERT_EQ(vector_3uints.m_vector.size(), 3);
}

TEST(bitvector, one_bit_set)
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

TEST(bitvector, unset_set_full)
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

TEST(bitvector, get_1bit)
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

TEST(bitvector_shift, single_uint)
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


TEST(bitvector_shift, three_uint)
{
    // 3 uints bitvector
    Bitvector<192> bv{};
    // Init
    for (uint64_t i{62} ; i<=65 ; i++) bv.set(i);
    for (uint64_t i{126} ; i<=129 ; i++) bv.set(i);

    // Initial_verification
    const uint64_t expected_uints[] {(0b11UL << 62), (0b11UL << 62) | 0b11UL , 0b11UL};
    for (size_t i{0} ; i<3 ; i++)
        ASSERT_EQ(bv.m_vector[i], expected_uints[i]);

    // Large shift : Everythng should be shifted of 1 position
    bv.toric_right_shift(0, 190);
    const uint64_t expected_large_shift[] {(0b1UL << 63), (0b1UL << 63) | 0b111UL , 0b111UL};
    for (size_t i{0} ; i<3 ; i++)
        ASSERT_EQ(bv.m_vector[i], expected_large_shift[i]);

    // Reinit
    bv.clear();
    for (uint64_t i{62} ; i<=65 ; i++) bv.set(i);
    for (uint64_t i{126} ; i<=129 ; i++) bv.set(i);

    // Large shift : left half of the vector should be 1 position shifted
    bv.toric_right_shift(0, 100);
    const uint64_t expected_half_shift[] {(0b1UL << 63), (0b11UL << 62) | 0b111UL , 0b11UL};
    for (size_t i{0} ; i<3 ; i++)
        ASSERT_EQ(bv.m_vector[i], expected_half_shift[i]);

    // Reinit
    bv.clear();
    for (uint64_t i{62} ; i<=65 ; i++) bv.set(i);
    for (uint64_t i{126} ; i<=129 ; i++) bv.set(i);

    // Truncated shift : shift on the uint border
    bv.toric_right_shift(63, 100);
    const uint64_t expected_trunckated_shift[] {(0b1UL << 62), (0b11UL << 62) | 0b111UL , 0b11UL};
    for (size_t i{0} ; i<3 ; i++)
    {
        ASSERT_EQ(bv.m_vector[i], expected_trunckated_shift[i]);
    }
}


TEST(bitvector_shift, toric)
{
    // 2 uints bitvector
    Bitvector<128> bv{};
    // Init
    for (uint64_t i{126} ; i<=129 ; i++) bv.set(i%128);

    // Init checks
    ASSERT_EQ(bv.m_vector[0], 0b11UL);
    ASSERT_EQ(bv.m_vector[1], 0b11UL << 62);

    // Large shift : Everythng should be shifted of 1 position
    bv.toric_right_shift(120, 10);
    ASSERT_EQ(bv.m_vector[0], 0b111UL);
    ASSERT_EQ(bv.m_vector[1], 0b1UL << 63);

    // Reinit
    bv.clear();
    for (uint64_t i{126} ; i<=129 ; i++) bv.set(i%128);

    // Truncated shift : shift on the uint border
    bv.toric_right_shift(127, 10);
    ASSERT_EQ(bv.m_vector[0], 0b111UL);
    ASSERT_EQ(bv.m_vector[1], 0b1UL << 62);
}


TEST(bitvector_rank, single_uint)
{
    // 3 uints bitvector
    Bitvector<64> bv{};
    // Init
    for (uint64_t i{0} ; i<64 ; i+=8) { bv.set(i); bv.set(i+7); }

    ASSERT_EQ(bv.rank(0, 63), 16);
    ASSERT_EQ(bv.rank(7, 7), 1);
    ASSERT_EQ(bv.rank(31, 32), 2);
}

TEST(bitvector_rank, triple_uint)
{
    // 3 uints bitvector
    Bitvector<192> bv{};
    // Init
    for (uint64_t i{0} ; i<192 ; i+=8) { bv.set(i); bv.set(i+7); }

    // 2 uints ranks
    ASSERT_EQ(bv.rank(0, 127), 32);
    ASSERT_EQ(bv.rank(32, 95), 16);
    ASSERT_EQ(bv.rank(0, 64), 17);
    ASSERT_EQ(bv.rank(63, 127), 17);
    // 3 uints ranks
    ASSERT_EQ(bv.rank(0, 191), 48);
    ASSERT_EQ(bv.rank(32, 159), 32);
    ASSERT_EQ(bv.rank(63, 191), 33);
    ASSERT_EQ(bv.rank(0, 128), 33);
}

TEST(bitvector_rank, toric)
{
    // 3 uints bitvector
    Bitvector<256> bv{};
    // // Init
    for (uint64_t i{0} ; i<256 ; i+=8) { bv.set(i); bv.set(i+7); }

    ASSERT_EQ(bv.rank(128, 127), 64);
    ASSERT_EQ(bv.rank(64, 63), 64);
    ASSERT_EQ(bv.rank(255, 0), 2);
}


TEST(bitvector_select, single_uint)
{
    // 3 uints bitvector
    Bitvector<64> bv{};
    // Init
    for (uint64_t i{0} ; i<64 ; i+=8) { bv.set(i); bv.set(i+7); }


    for (uint64_t i{0} ; i<(64/8) ; i+=1)
    {
        ASSERT_EQ(bv.select(0, 2*i+1), 8*i  );
        ASSERT_EQ(bv.select(0, 2*i+2), 8*i+7);
    }
}

TEST(bitvector_select, toric)
{
    // 3 uints bitvector
    Bitvector<128> bv{};
    // Init
    for (uint64_t i{0} ; i<128 ; i+=8) { bv.set(i); bv.set(i+7); }


    for (uint64_t i{0} ; i<(128/8) ; i+=1)
    {
        ASSERT_EQ(bv.select(64, 2*i+1), (64 + 8 * i    ) % 128 );
        ASSERT_EQ(bv.select(64, 2*i+2), (64 + 8 * i + 7) % 128 );
    }
}


TEST(bitvector_select, multiple_uints)
{
    // 3 uints bitvector
    Bitvector<256> bv{};
    // // Init
    for (uint64_t i{0} ; i<256 ; i+=8) { bv.set(i); bv.set(i+7); }

    ASSERT_EQ(bv.select(0, 1), 0);
    ASSERT_EQ(bv.select(0, 64), 255);
    ASSERT_EQ(bv.select(128, 33), 0);
    ASSERT_EQ(bv.select(128, 34), 7);
    ASSERT_EQ(bv.select(128, 49), 64);
}


TEST(bitvector_first_one, single_uint)
{
    Bitvector<64> bv{};

    // Set all the 2^i bits
    for (uint64_t i{0} ; i<6 ; i++)
        bv.set(1UL<<i);

    ASSERT_EQ(bv.first_one(0), 1);
    ASSERT_EQ(bv.first_one(2), 2);
    ASSERT_EQ(bv.first_one(3), 4);
    ASSERT_EQ(bv.first_one(5), 8);
    ASSERT_EQ(bv.first_one(9), 16);
    ASSERT_EQ(bv.first_one(17), 32);
    ASSERT_EQ(bv.first_one(33), 1);
    ASSERT_EQ(bv.first_one(63), 1);
}

TEST(bitvector_first_one, multiple_uints)
{
    Bitvector<1024> bv{};

    // Set all the 2^i bits
    for (uint64_t i{0} ; i<10 ; i++)
        bv.set(1UL<<i);

    ASSERT_EQ(bv.first_one(0), 1);
    ASSERT_EQ(bv.first_one(2), 2);
    ASSERT_EQ(bv.first_one(3), 4);
    ASSERT_EQ(bv.first_one(5), 8);
    ASSERT_EQ(bv.first_one(9), 16);
    ASSERT_EQ(bv.first_one(17), 32);
    ASSERT_EQ(bv.first_one(33), 64);
    ASSERT_EQ(bv.first_one(65), 128);
    ASSERT_EQ(bv.first_one(129), 256);
    ASSERT_EQ(bv.first_one(257), 512);
    ASSERT_EQ(bv.first_one(513), 1);
    ASSERT_EQ(bv.first_one(1023), 1);
}