#include <iostream>
#include <cstdlib>
#include <gtest/gtest.h>
#include <io/KmerStream.hpp>

static const std::string data_path {std::getenv("DBGLIB_TEST_DATA") == nullptr ? "" : std::getenv("DBGLIB_TEST_DATA")};

static const std::string fa0 {"tests/data/fasta0.fa"};
class KmerStreamIn_fa0_Fixture : public testing::Test , public KmerStreamIn
{
protected:

public:
    KmerStreamIn_fa0_Fixture() : KmerStreamIn(fa0)
    {};

};


TEST_F(KmerStreamIn_fa0_Fixture, init)
{
    std::cout << "path " << data_path << std::endl;
    ASSERT_EQ(0, 2 - 2);
}
