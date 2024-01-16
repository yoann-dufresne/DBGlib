#include <iostream>
#include <cstdlib>
#include <gtest/gtest.h>
#include <string>

#include <io/Skmer.hpp>

using namespace std;


TEST(SkmerManipulator, enumerate)
{
	using kuint = uint8_t;
    using kpair = km::Skmer<kuint>::pair<kuint>;
    constexpr uint64_t k{4};
    constexpr uint64_t m{1};

	const string seq {"TCAAGCACTAA"};
    //                            prefix:        _   _       _   _  ,     _   _       _   _  ,     _   _       _   _  ,
    //                            suffix:          _       _   T    ,       _       T   C    ,       T       C   A    ,
    const kuint expected_fwd_values[][2] = {{0b00000000, 0b00001000},{0b00000000, 0b10000100},{0b00001000, 0b01000000},
    //                                           T   _       _   _  ,     C   T       _   _  ,     A   C       T   _  ,
    //                                             C       A   A    ,       A       A   G    ,       A       G   C    ,
                                            {0b00100100, 0b00000000},{0b00010010, 0b00001100},{0b00000001, 0b11100100},
    //                                           A   A       C   T  ,     G   A       A   C  ,     C   G       A   A  ,
    //                                             G       C   A    ,       C       A   C    ,       A       C   T    ,
                                            {0b00001100, 0b01010010},{0b00110100, 0b00000101},{0b00010011, 0b01001000},
    //                                           A   C       G   A  ,     C   A       C   G  
    //                                             C       T   A    ,       T       A   A    
                                            {0b00000101, 0b10110000},{0b00011000, 0b00010011}};

    km::SkmerManipulator<kuint> manip {k, m};

    uint64_t idx{0};
    for (const auto& letter : seq)
    {
    	kuint nucl {static_cast<kuint>((letter >> 1) & 0b11)};
        [[maybe_unused]]const auto& min_skmer {manip.add_nucleotide(nucl)};

        //                            Less significant             Most significant
        const kpair expected_fwd_pair{expected_fwd_values[idx][1], expected_fwd_values[idx][0]};
        ASSERT_EQ(manip.m_fwd.m_pair, expected_fwd_pair);
        
        // EXPECT_EQ(manip.m_rev, expected_rev_values[idx]);

        // EXPECT_EQ(min_kmer, std::min(expected_fwd_values[idx], expected_rev_values[idx]));

        idx += 1;
    }
}
