#include <array>

#include <dbg/datastruct/PackedArray.hpp>

#ifndef QUOTIENT_FILTER_H
#define QUOTIENT_FILTER_H

template<uint64_t q, uint64_t r, typename Quotienting>
class QuotientFilter
{
protected:
    static constexpr uint64_t num_blocks {1UL << (q - 7)};
    std::array<PackedBlock, QuotientFilter::num_blocks> rests {};
public:
    QuotientFilter()
    {

    };
};

#endif
