#include <array>

#include <dbg/datastruct/PackedArray.hpp>

#ifndef QUOTIENT_FILTER_H
#define QUOTIENT_FILTER_H

template<uint64_t q, uint64_t r, typename Quotienting>
class QuotientFilter
{
protected:
    // Number of blocks. Defined at compile time
    static constexpr uint64_t num_blocks {1UL << (q - 6)};
    // Storage area of rests
    std::array<PackedBlock<r>, QuotientFilter::num_blocks> m_rests {};
    // Occupied bitvector
    std::array<uint64_t, QuotientFilter::num_blocks> m_occupied {};
    // Run ends bitvector
    std::array<uint64_t, QuotientFilter::num_blocks> m_runend {};
    // Offset vector
    std::array<uint64_t, QuotientFilter::num_blocks> m_offsets {};
    // Quotienting class
    Quotienting m_quotienting {};

public:
    QuotientFilter()
    {
        static_assert(q >= 6, "Minimum structure size is 64 elements => q must be larger or equal to 6");
        static_assert(r < 64, "Rest size must be < 64 to be stored in uint64_t arrays");
    };

    void insert(uint64_t value)
    {
        const auto qr {m_quotienting.compute<q, r>(value)};
        const auto block {qr.quotient >> 6};
    }
};

#endif
