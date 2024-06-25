#include <iostream>
#include <cstdlib>
#include <gtest/gtest.h>
#include <string>

#include <io/Skmer.hpp>
#include <io/Skmerator.hpp>
#include <algorithms/SkmerSorting.hpp>

using namespace std;

using kuint = uint16_t;
using kpair = km::Skmer<kuint>::pair;

constexpr uint64_t k{5};
constexpr uint64_t m{2};

km::SkmerManipulator<kuint> manip {k, m};
km::SkmerPrettyPrinter<kuint> pp {k, m};

    //                      Prefix:      A   _   _   _            A   _   _   _            C   _   _   _
    //                      Suffix:    A   A   A   A            A   C   C   C            C   C   C   C 
const std::array< kpair, 3> kmer_triplet { kpair(0b0000001100110011U,0), kpair(0b0000011101110111U,0), kpair(0b0101011101110111U,0) };


std::array< std::array< km::Skmer<kuint>, 3>, 6> get_skmer_permutations (std::array< kpair, 3> const & kmer_triplet)
{

    std::array< std::array< km::Skmer<kuint>, 3>, 6> permutation_array {};

    permutation_array[0] = {km::Skmer<kuint>(kmer_triplet[0],0,3),
                            km::Skmer<kuint>(kmer_triplet[1],0,3),
                            km::Skmer<kuint>(kmer_triplet[2],0,3)};
    for(auto el: permutation_array[0]){
        pp << el;
        std::cout << el << std::endl;
    }

    for(int64_t i {1}; i < 6; i++){
        permutation_array[i] = permutation_array[i-1];
        std::next_permutation(permutation_array[i].begin(), permutation_array[i].end());
    }

    return permutation_array;
}



TEST(SkmerSorting, kmer_validation)
{
    //                         Prefix:         A   T   _   _             A   _   _   _   
    //                         Suffix:       A   C   C   C             C   C   C   C     
    const kpair input_skmers[2] { {0, 0b0000011001110111U}, {0, 0b0100011101110111U}} ;
    // km::Skmer<kuint> sk2(input_skmers[1],1,4);
    //const km::Skmer<kuint> skmers_array[2] = {sk1,sk2} ;
    std::vector<km::Skmer<kuint>> m_skmer_vector{km::Skmer<kuint>(input_skmers[0],2,4), km::Skmer<kuint>(input_skmers[1],1,4)};
    // std::cout << "Before change: " <<  m_skmer_vector[0].m_pref_size << std::endl;
    // m_skmer_vector[0].m_pref_size = 5;
    // std::cout << "After change: " << m_skmer_vector[0].m_pref_size << std::endl;
    // m_skmer_vector.push_back(sk1);


    const uint64_t kmer_positions {(2*k - m)};

    const uint64_t expected_valid_kmers[kmer_positions][2]{{0,0},{0,0},{1,0},{1,1},{1,1},{1,1},{1,1},{1,1}};
    // 0 values map to false, else to true
    bool kmer_validity;
    for (uint64_t skmer_id {0}; skmer_id < 2; skmer_id++ ){
        for(uint64_t position; position < kmer_positions; position++ ){
            kmer_validity = manip.has_valid_kmer(m_skmer_vector[skmer_id], position);
            ASSERT_EQ(kmer_validity, expected_valid_kmers[position][skmer_id]);
        }
    }
}

TEST(SkmerSorting, Single_kmer_sorting)
{
    
    //                         Prefix:         A   T   _   _             A   _   _   _   
    //                         Suffix:       A   C   C   C             C   C   C   C     
    const kpair input_skmers[2] { {0, 0b0000011001110111U}, {0, 0b0100011101110111U}} ;
    
    std::vector<km::Skmer<kuint> > m_skmer_vector{km::Skmer<kuint>(input_skmers[1],1,4), km::Skmer<kuint>(input_skmers[0],2,4)};

    //const uint64_t kmer_positions {(2*k - m)};
    const uint64_t position {3};
    std::vector<uint64_t> ordered_kmers = km::sorting::sort_column(m_skmer_vector.begin(), m_skmer_vector.end(), position, manip);
    std::vector<uint64_t> expected_order {1, 0};

    ASSERT_EQ(ordered_kmers,expected_order);
}

TEST(SkmerSorting, Three_kmer_sorting)
{
    const uint64_t position {3};
    std::vector<uint64_t> ordered_kmers {};

    for(auto const & permuted: get_skmer_permutations(kmer_triplet)){

        ordered_kmers = km::sorting::sort_column(permuted.begin(), permuted.end(), position, manip);
        uint64_t loop_idx {0};

        for (auto & el: permuted){
            pp << el;
            std:: cout << pp;
        }
        
        for(auto & positon: ordered_kmers){
            km::Skmer<kuint> const & curr_skmer { permuted[position] };
            kpair const & expected_value {kmer_triplet[loop_idx]};
            ASSERT_EQ(curr_skmer.m_pair,expected_value);

            loop_idx+=1;
        }
    }

}