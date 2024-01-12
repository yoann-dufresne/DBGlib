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
    //                                      T _ _ _ _     C T _ _ _     A C T _ _     A A C T _     G A A C T
    const kuint expected_fwd_values[] = { 0b1000000000, 0b0110000000, 0b0001100000, 0b0000011000, 0b1100000110
    //                                      C G A A C     A C G A A     C A C G A     T C A C G
    									, 0b0111000001, 0b0001110000, 0b0100011100, 0b1001000111};
    //                                      _ _ _ _ A     _ _ _ A G     _ _ A G T     _ A G T T     A G T T C
    const kuint expected_rev_values[] = { 0b0000000000, 0b0000000011, 0b0000001110, 0b0000111010, 0b0011101001
    //                                      G T T C G     T T C G T     T C G T G     C G T G A
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
