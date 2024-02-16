#include <array>
#include <cstdint>

#include <dbg/datastruct/PackedArray.hpp>
#include <dbg/datastruct/bitvector.hpp>
#include <dbg/datastruct/quotienting.hpp>

#ifndef QUOTIENT_FILTER_H
#define QUOTIENT_FILTER_H

template<uint64_t q, uint64_t r, class Quotienting>
class QuotientFilter
{
protected:
    // Number of blocks. Defined at compile time
    static constexpr uint64_t num_blocks {1UL << (q - 6)};
    // Quotienting class
    Quotienting m_quotienting {};
public:
    // Storage area of rests
    std::array<PackedBlock<r>, QuotientFilter::num_blocks> m_rests {};
    // Occupied bitvector
    Bitvector<1UL << q> m_occupied {};
    // Run ends bitvector
    Bitvector<1UL << q> m_runend {};
    // Offset vector
    std::array<uint64_t, QuotientFilter::num_blocks> m_offsets {};

    QuotientFilter()
    {
        static_assert(64 >= q + r);
        static_assert(q >= 6, "Minimum structure size is 64 elements => q must be larger or equal to 6");
        static_assert(r < 64, "Rest size must be < 64 to be stored in uint64_t arrays");
    };

    void insert(const uint64_t& value)
    {
        const auto qr {m_quotienting.compute<q, r>(value)};
        const auto block {qr.quotient >> 6};

        // What shit do I need to apply to find where to insert the rest ?
        // Do the insertion

        m_occupied.set(value);
    }

    /** Insert the rest in the QF at the address represented by the quotient. Do not test the slot availablity and
     * insert as if it was free.
     * @param qr The quotient/rest split of the value.
     **/
    void insert_in_free_space(const QR<q, r>& qr)
    {
        // Set metadata for the new run
        m_occupied.set(qr.quotient);
        m_runend.set(qr.quotient);

        // Set the rest
        const uint64_t block_idx {static_cast<uint64_t>(qr.quotient) >> 6};
        const uint64_t relative_idx {qr.quotient & 0b111111UL};
        m_rests[block_idx].set(relative_idx, qr.rest);
    }

    /** Insert the rest at the insertion_idx position. Performs a cascading shift to move the rests that can already be
     * in place. /!\ The function do not add metadata bits for the rest inserted. It has to be done externaly if needed.
     * @param rest the rest to insert.
     * @param insertion_idx Index where the rest must be inserted
     * @param first_free_slot Last slot where the function write the casading slots shifted.
     **/
    void insert_and_shift(const uint64_t rest, const uint64_t insertion_idx, const uint64_t first_free_slot)
    {
        bool inserted {false};
        uint64_t to_insert {rest};

        uint64_t current_idx {insertion_idx};
        uint64_t current_block_index {insertion_idx >> 6};
        uint64_t current_64_index {insertion_idx & 0b111111};
        
        PackedBlock<r>& current_block = m_rests[current_block_index];

        // Loop over the slots to shift everything until we reach an empty slot
        while (first_free_slot != current_idx)
        {
            // Rewrite the current slot
            const uint64_t save {current_block.get(current_64_index)};
            current_block.set(current_64_index, to_insert);
            to_insert = save;

            // Jump to the next slot
            current_idx += 1;
            current_64_index = (current_64_index + 1) & 0b111111;

            // Block change
            if (current_64_index == 0)
            {
                current_block_index += 1;

                // In casee we hit the right of the datastructure => toricity
                if ((1UL << q) == current_idx)
                {
                    current_idx = 0;
                    current_block_index = 0;
                }

                // Getting the current block
                current_block = m_rests[current_block_index];
            }
        }

        // Write the last value
        current_block.set(current_64_index, to_insert);

        // Shift the runend bits from the run moved
        m_runend.toric_shift(insertion_idx, first_free_slot);
    }
};

#endif
