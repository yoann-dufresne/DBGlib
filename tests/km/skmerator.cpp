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

    km::SkmerManipulator<kuint> manip {5,2};
    km::FileSkmerator<kuint> skmerator {fa3, manip};

    //const kuint expected_values[5] { 0b0000010100000111, 0b00010100001101011, 0b0001010001110011, 0b00010101011001111, 0b0000110111001101 };

    // FOR FASTA_3 WE EXPECT
    //const kuint expected_values_fa3[2] {0b}

    uint64_t nb_skmer {0};
    for ([[maybe_unused]]km::Skmer<kuint> skmer : skmerator)
    {
        cout << manip << endl;
        //EXPECT_EQ(expected_values[nb_skmer], manip.m_fwd.m_pair);
        nb_skmer += 1;
    }
 
    EXPECT_EQ(nb_skmer, 3);
}

// TODO: TEST THE MASKING OF THE SKMERS