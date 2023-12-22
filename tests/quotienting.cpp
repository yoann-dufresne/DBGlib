#include <gtest/gtest.h>

#include <dbg/datastruct/quotienting.hpp>


#include <iostream>
using namespace std;


TEST(quotientpair, quotientpair_init)
{
  QR<6, 6> qr {42, 17};

  ASSERT_EQ(qr.quotient, 42);
  ASSERT_EQ(qr.rest, 17);
}

TEST(quotientpair, quotientpair_overflow_init)
{
  QR<6, 6> qr {64+27, 64+43};

  ASSERT_EQ(qr.quotient, 27);
  ASSERT_EQ(qr.rest, 43);
}

