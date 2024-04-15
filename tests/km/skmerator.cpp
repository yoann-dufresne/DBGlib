#include <iostream>
#include <cstdlib>
#include <gtest/gtest.h>
#include <string>

#include <io/Skmer.hpp>
#include <io/Skmerator.hpp>

using namespace std;


static const std::string data_path {std::getenv("DBGLIB_TEST_DATA") == nullptr ? "" : std::getenv("DBGLIB_TEST_DATA")};

// TODO: tmp usage of relative path befor solving the getenv issue.
static const std::string fa1 {"../tests/data/fasta1.fa"};
static const std::string fa2 {"../tests/data/fasta2.fa"};
static const std::string fa3 {"../tests/data/fasta3.fa"};

TEST(Skmerator, two_nucleotides)
{
    using kuint = uint16_t;
    const uint64_t k{5};
    const uint64_t m{2};

    km::SkmerManipulator<kuint> manip {k, m};
    km::FileSkmerator<kuint> skmerator {fa3, manip};
    km::SkmerPrettyPrinter<kuint> pp {k, m};

    //                         Prefix:          C   C   C   C             A   C   C   C             A   A   C   
    //                         Suffix:       A   _   _   _             A    A   _   _             A   A   A   _
    const kuint expected_values[][2] { {0, 0b0001110111011101U}, {0, 0b0000000111011101U}, {0, 0b00000000000111U}
    };

    // FOR FASTA_3 WE EXPECT
    //const kuint expected_values_fa3[2] {0b}

    uint64_t nb_skmer {0};
    for ([[maybe_unused]]km::Skmer<kuint> skmer : skmerator)
    {
        pp << &skmer;
        cout << pp << endl;
        //EXPECT_EQ(expected_values[nb_skmer], manip.m_fwd.m_pair);
        nb_skmer += 1;
    }
 
    ASSERT_EQ(nb_skmer, 3);
}

// TODO: TEST THE MASKING OF THE SKMERS