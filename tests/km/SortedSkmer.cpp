#include <iostream>
#include <cstdlib>
#include <gtest/gtest.h>
#include <string>

#include <io/Skmer.hpp>
#include <io/Skmerator.hpp>
#include <skmer/SkmerSorting.hpp>

using namespace std;

TEST(SkmerSorting, kmer_validation)
{
    using kuint = uint16_t;
    using kpair = km::Skmer<kuint>::pair;
    
    const uint64_t k{5};
    const uint64_t m{2};

    km::SkmerManipulator<kuint> manip {k, m};
    km::SkmerPrettyPrinter<kuint> pp {k, m};
    
    //                         Prefix:         A   T   _   _             A   _   _   _   
    //                         Suffix:       A   C   C   C             C   C   C   C     
    const kpair input_skmers[2] { {0, 0b0000011001110111U}, {0, 0b0100011101110111U}} ;
    km::Skmer<kuint> sk1(input_skmers[0],2,4);
    km::Skmer<kuint> sk2(input_skmers[1],1,4);
    const km::Skmer<kuint> skmers_array[2] = {sk1,sk2} ;
    
    const uint64_t kmer_positions {(2*k - m)};

    const uint64_t expected_valid_kmers[kmer_positions][2]{{0,0},{0,0},{1,0},{1,1},{1,1},{1,1},{1,1},{1,1}};
    // 0 values map to false, else to true
    bool kmer_validity;
    for (uint64_t skmer_id {0}; skmer_id < 2; skmer_id++ ){
        for(uint64_t position; position < kmer_positions; position++ ){
            kmer_validity = km::has_valid_skmer(manip, position,skmers_array[skmer_id]);
            ASSERT_EQ(kmer_validity, expected_valid_kmers[position][skmer_id]);
        }
    }
}

TEST(SkmerSorting, Single_kmer_sorting)
{
    using kuint = uint16_t;
    using kpair = km::Skmer<kuint>::pair;
    
    const uint64_t k{5};
    const uint64_t m{2};

    km::SkmerManipulator<kuint> manip {k, m};
    km::SkmerPrettyPrinter<kuint> pp {k, m};
    
    //                         Prefix:         A   T   _   _             A   _   _   _   
    //                         Suffix:       A   C   C   C             C   C   C   C     
    const kpair input_skmers[2] { {0, 0b0000011001110111U}, {0, 0b0100011101110111U}} ;
    
    km::Skmer<kuint> sk1(input_skmers[0],2,4);
    km::Skmer<kuint> sk2(input_skmers[1],1,4);
    km::Skmer<kuint> skmers_array[2] {sk1,sk2};


    //const uint64_t kmer_positions {(2*k - m)};

    std::vector<uint64_t> ordered_kmers = km::sort_kmers(manip,skmers_array);
    //std::vector<uint64_t> expected_order {0,1};

    //ASSERT_EQ(ordered_kmers,expected_order);
}