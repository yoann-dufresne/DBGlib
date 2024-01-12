#include <iostream>
#include <cstdlib>
#include <gtest/gtest.h>
#include <string>

#include <io/Kmer.hpp>

using namespace std;


TEST(KmerManipulator, enumerate)
{
	using kuint = uint16_t;

	const string seq {"TCAAGCACT"};
    const kuint expected_fwd_values[] = { 0b1000000000, 0b0110000000, 0b0001100000, 0b0000011000, 0b1100000110
    									, 0b0111000001, 0b0001110000, 0b0100011100, 0b1001000111};
    const kuint expected_rev_values[] = { 0b0000000000, 0b0000000011, 0b0000001110, 0b0000111010, 0b0011101001
    									, 0b1110100111, 0b1010011110, 0b1001111011, 0b0111101100};

	km::KmerManipulator<kuint> manip {5};

    uint64_t idx{0};
    for (const auto& letter : seq)
    {
    	kuint nucl {static_cast<kuint>((letter >> 1) & 0b11)};
        const auto& min_kmer {manip.add_nucleotide(nucl)};
        EXPECT_EQ(manip.m_fwd, expected_fwd_values[idx]);
        EXPECT_EQ(manip.m_rev, expected_rev_values[idx]);

        EXPECT_EQ(min_kmer, std::min(expected_fwd_values[idx], expected_rev_values[idx]));

        idx += 1;
    }
}
