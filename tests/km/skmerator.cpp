#include <iostream>
#include <cstdlib>
#include <gtest/gtest.h>
#include <string>

#include <io/Skmer.hpp>
#include <io/Skmerator.hpp>

using namespace std;


// static const std::string data_path {std::getenv("DBGLIB_TEST_DATA") == nullptr ? "" : std::getenv("DBGLIB_TEST_DATA")};

// TODO: tmp usage of relative path befor solving the getenv issue.
// static const std::string fa1 {"../tests/data/fasta1.fa"};
// static const std::string fa2 {"../tests/data/fasta2.fa"};
// static const std::string fa3 {"../tests/data/fasta3.fa"};

TEST(Skmerator, decreasing_minimizer)
{
    using kuint = uint16_t;
    using kpair = km::Skmer<kuint>::pair;
    
    const uint64_t k{5};
    const uint64_t m{2};

    km::SkmerManipulator<kuint> manip {k, m};
    std::string seq{"CCCCAAAAA"};
    km::SeqSkmerator<kuint> skmerator {manip, seq};
    km::SkmerPrettyPrinter<kuint> pp {k, m};
    

    //                         Prefix:         C   C   C   C             A   C   C   C             A   A   C   _  
    //                         Suffix:       A   _   _   _             A    A   _   _             A   A   A   _
    const kuint expected_values[][2] { {0, 0b0001110111011101U}, {0, 0b0000000111011101U}, {0, 0b0000000000011111U}
    };

    // FOR FASTA_3 WE EXPECT
    //const kuint expected_values_fa3[2] {0b}

    uint64_t nb_skmer {0};
    for ([[maybe_unused]]km::Skmer<kuint> skmer : skmerator)
    {
        // pp << &skmer;
        // cout << pp << endl;

        //                            Less significant             Most significant
        const kpair expected_pair{expected_values[nb_skmer][1], expected_values[nb_skmer][0]};
        ASSERT_EQ(expected_pair, skmer.m_pair);
        
        nb_skmer += 1;
    }
    EXPECT_EQ(nb_skmer, 3);
}


TEST(Skmerator, increasing_minimizer)
{
    using kuint = uint16_t;
    using kpair = km::Skmer<kuint>::pair;
    
    const uint64_t k{5};
    const uint64_t m{2};

    km::SkmerManipulator<kuint> manip {k, m};
    std::string seq{"AACCCC"};
    km::SeqSkmerator<kuint> skmerator {manip, seq};
    km::SkmerPrettyPrinter<kuint> pp {k, m};
    

    //                         Prefix:         A   _   _   _             A   _   _   _   
    //                         Suffix:       A   C   C   C             C   C   C   C     
    const kuint expected_values[][2] { {0, 0b0000011101110111U}, {0, 0b0100011101110111U}
    };

    // FOR FASTA_3 WE EXPECT
    //const kuint expected_values_fa3[2] {0b}

    uint64_t nb_skmer {0};
    for ([[maybe_unused]]km::Skmer<kuint> skmer : skmerator)
    {
        pp << &skmer;
        cout << pp << endl;

        //                            Less significant             Most significant
        const kpair expected_pair{expected_values[nb_skmer][1], expected_values[nb_skmer][0]};
        ASSERT_EQ(expected_pair, skmer.m_pair);
        
        nb_skmer += 1;
    }

    EXPECT_EQ(nb_skmer, 2);
}