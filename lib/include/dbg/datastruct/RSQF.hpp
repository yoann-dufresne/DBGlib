#include <array>

#include <dbg/datastruct/PackedArray.hpp>

#ifndef QUOTIENT_FILTER_H
#define QUOTIENT_FILTER_H

template<uint64_t q, uint64_t r, typename Quotienting>
class QuotientFilter
{
protected:
    static constexpr uint64_t num_blocks {1UL << (q - 6)};
    std::array<PackedBlock<r>, QuotientFilter::num_blocks> rests {};
    Quotienting quotienting {};

public:
    QuotientFilter()
    {
        static_assert(q >= 6, "Minimum structure size is 64 elements => q must be larger or equal to 6");
        static_assert(r < 64, "Rest size must be < 64 to be stored in uint64_t arrays");
    };
};

#endif
