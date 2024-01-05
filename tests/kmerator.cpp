#include <iostream>
#include <cstdlib>
#include <gtest/gtest.h>
#include <string>

#include <io/Kmerator.hpp>

using namespace std;


static const std::string data_path {std::getenv("DBGLIB_TEST_DATA") == nullptr ? "" : std::getenv("DBGLIB_TEST_DATA")};

// TODO: tmp usage of relative path befor solving the getenv issue.
static const std::string fa0 {"../tests/data/fasta0.fa"};


TEST(Kmerator, init)
{
    FileKmerator<5, uint64_t> kmerator {fa0};

    for (auto& kmer : kmerator)
        cout << kmer.value << endl;
 
    EXPECT_EQ(0, 2);
}
