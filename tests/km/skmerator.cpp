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

TEST(Skmerator, init)
{
    using kuint = uint64_t;

    km::SkmerManipulator<kuint> manip {5,2};
    km::FileSkmerator<kuint> skmerator {fa1, manip};

    uint64_t nb_skmer {0};
    for ([[maybe_unused]]km::Skmer<kuint> skmer : skmerator)
    {
        cout << manip << endl;
        nb_skmer += 1;
        ASSERT_EQ(0,1);
    }
 
    EXPECT_EQ(nb_skmer, 24);
}