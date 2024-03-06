#include <cstdint>
#include <array>
#include <bit>
#include <immintrin.h>


#ifndef BITVEC_H
#define BITVEC_H


template <uint64_t size>
class Bitvector
{
	static constexpr auto create_set_masks()
	{
		std::array<uint64_t, 64> array {};
		for (int i = 0; i < 64; ++i)
		{
			array[i] = 1UL << i;
		}
		return array;
	}
	static constexpr auto set_masks = create_set_masks();
	static constexpr auto create_unset_masks()
	{
		std::array<uint64_t, 64> array {};
		for (int i{0} ; i<64 ; i++) {
			array[i] = ~set_masks[i];
		}
		return array;
	}
	static constexpr auto unset_masks = create_unset_masks();

	static constexpr uint64_t num_uint {size / 64};
	
public:
	std::array<uint64_t, (size+63UL)/64> m_vector {};

	Bitvector () {};

	/** Get the position of the first bit set to 1, starting at position from. The search is toric.
	 * @param from Bit position where to start the search.
	 * @return The absolute position in the bitvector
	 **/
	uint64_t first_one(const uint64_t from) const
	{
		uint64_t mask {(~static_cast<uint64_t>(0)) << (from % 64UL)};
		uint64_t uint_idx {from / 64UL};

		// First uint check
		uint64_t value {m_vector[uint_idx] & mask};
		if (value == 0)
		{
			uint_idx += 1;
			if (uint_idx == num_uint)
				uint_idx = 0;
			value = m_vector[uint_idx];
		}

		// Following uints
		while (m_vector[uint_idx] == 0)
		{
			uint_idx += 1;
			if (uint_idx == num_uint)
				uint_idx = 0;
			value = m_vector[uint_idx];
		}

		// Get the first one position
		return uint_idx * 64UL + std::countr_zero(value);
	}

	void clear()
	{
		for (size_t i{0} ; i<m_vector.size() ; i++)
			m_vector[i] = 0;
	}

	inline uint64_t get_size() const
	{
		return size;
	}

	void set(const uint64_t index)
	{
		const uint64_t vector_index {index / 64};
		m_vector[vector_index] |= set_masks[index % 64];
	};

	void set(const uint64_t index, bool val)
	{
		if (val)
			set(index);
		else
			unset(index);
	};

	void unset(const uint64_t index)
	{
		const uint64_t vector_index {index / 64};
		m_vector[vector_index] &= unset_masks[index % 64];
	};

	bool get(const uint64_t index) const
	{
		return (m_vector[index / 64] & set_masks[index % 64]) != 0;
	}

	/** Count the number of bits set to 1 from start_idx to rank_idx. This rank operation is toric.
	 * /!\ Known limitation: You cannot rank the whole bitvector in a toric way. If you are asking results on toric
	 * query, the ranked bit must be in a different uint than the start bit.
	 * @param start_idx The bit index from which the rank should start (Bit 0 in classic rank)
	 * @param rank_idx Position in the bitvector where to stop the ranking (position included)
	 * 
	 * @return The number of 1s found between the 2 boundaries (included)
	 **/
	uint64_t rank(const uint64_t start_idx, const uint64_t rank_idx) const
	{
		const uint64_t start_uint {start_idx / 64};
		uint64_t current_uint {start_uint};
		const uint64_t stop_uint {rank_idx / 64};

		// --- Both limits are in the same uint ---
		if (start_uint == stop_uint)
		{
			// Create a mask
			const uint64_t start_mask {(~static_cast<uint64_t>(0)) << (start_idx % 64)};
			const uint64_t stop_mask {(~static_cast<uint64_t>(0)) >> (63 - (rank_idx % 64))};
			const uint64_t mask {start_mask & stop_mask};

			// Return the number of 1s
			return std::popcount<uint64_t>(m_vector[start_uint] & mask);
		}

		// --- Multiple uint coverage ---
		// Start uint
		const uint64_t start_mask {(~static_cast<uint64_t>(0)) << (start_idx % 64)};
		uint64_t num_ones {static_cast<uint64_t>(std::popcount(m_vector[start_uint] & start_mask))};

		if (current_uint+1 == num_uint)
			current_uint = 0;
		else
			current_uint += 1;

		// Middle uints
		while (current_uint != stop_uint)
		{
			num_ones += std::popcount(m_vector[current_uint]);

			// uint increment
			if (current_uint+1 == num_uint)
				current_uint = 0;
			else
				current_uint += 1;
		}

		// Last uint
		const uint64_t stop_mask {(~static_cast<uint64_t>(0)) >> (63 - (rank_idx % 64))};
		num_ones += std::popcount(m_vector[stop_uint] & stop_mask);

		return num_ones;
	}

	/** Return the position of the val-th bit set starting from index start_idx. The answer is computed using toricity
	 * if needed.
	 * /!\ Limitation: val == 0 leads to undefined behaviour.
	 * @param start_idx The bit index from which the select should start (Bit 0 in classic select)
	 * @param val The number of bits to count (Must be > 0)
	 * 
	 * @return The absolute position of the val-th set bit.
	 **/
	uint64_t select(const uint64_t start_idx, const uint64_t num_1s) const
	{
		const uint64_t start_uint {start_idx / 64};
		uint64_t current_uint {start_uint};
		uint64_t remaining_1s {num_1s};

		// --- Multiple uint coverage ---
		// Start uint
		const uint64_t start_mask {(~static_cast<uint64_t>(0)) << (start_idx % 64)};
		const uint64_t buffered_uint {m_vector[start_uint] & start_mask};
		const uint64_t first_1s {static_cast<uint64_t>(std::popcount(buffered_uint))};

		// Update select
		if (first_1s >= remaining_1s)
			return current_uint * 64UL + select64(buffered_uint, num_1s);
		else
			remaining_1s -= first_1s;

		// Update block
		if (current_uint+1 == num_uint)
			current_uint = 0;
		else
			current_uint += 1;

		// Middle uints
		while (current_uint != start_uint)
		{
			const uint64_t local_1s {static_cast<uint64_t>(std::popcount(m_vector[current_uint]))};

			// Update select
			if (local_1s >= remaining_1s)
				return current_uint * 64UL + select64(m_vector[current_uint], remaining_1s);
			else
				remaining_1s -= local_1s;

			// uint increment
			if (current_uint+1 == num_uint)
				current_uint = 0;
			else
				current_uint += 1;
		}

		return size;
	}

	static uint64_t select64 (const uint64_t vector, const uint64_t num_1s)
	{
		const uint64_t pdep_val {_pdep_u64(1ULL << (num_1s-1), vector)};
		return std::countr_zero<uint64_t>(pdep_val);
	}

	/** Shift the bitvector 1 bit to the right. from and to are included positions. Left bit is 0.
	 * Right bit overflowing is discarded.
	 * /!\ Do not work on toric queries that starts and ends in the same uint.
	 * @param from First bit to shift.
	 * @param to Last bit to shift (and to discard).
	 **/
	void toric_right_shift(const uint64_t from, const uint64_t to)
	{
		// std::cout << "shift from " << from << " to " << to << std::endl;
		const uint64_t first_uint {from/64UL};
		const uint64_t last_uint {to/64UL};
		uint64_t current_uint {first_uint};

		//  --- everything is in the same uint ---
		if ((first_uint == last_uint) and (from < to))
		{
			// - Prepare slice -
			// less significant part of the mask
			const uint64_t slice_right_mask {(~static_cast<uint64_t>(0UL)) >> (63 - (from % 64))};
			// most significant part of the mask
			const uint64_t left_mask {(~static_cast<uint64_t>(0UL)) >> (63 - (to % 64))};
			const uint64_t slice_full_mask {slice_right_mask ^ left_mask};
			// Get the slice to shift and shift it
			const uint64_t shifted_slice {(m_vector[current_uint] << 1 ) & slice_full_mask};

			// - Prepare vector -
			// less significant part of the mask
			const uint64_t vector_right_mask {slice_right_mask >> 1};
			// most significant part of the mask
			const uint64_t vector_full_mask {~(left_mask ^ vector_right_mask)};
			// Remove the bits from the slice to shift
			m_vector[current_uint] &= vector_full_mask;

			// - Merge everything -
			m_vector[current_uint] |= shifted_slice;

			return;
		}

		// --- Multiple uint to modify ---
		uint64_t carry_bit {0};
		while (current_uint != last_uint)
		{
			// - Mask the beginning if this is the first uint to be shifted -
			if (current_uint == first_uint)
			{
				// Create mask to ignore less significant bits until from
				const uint64_t mask {(1UL << (from % 64)) - 1UL};
				// extract and shift the slice
				const uint64_t shifted_slice {(m_vector[current_uint] & ~mask) << 1};
				// Save the carry bit
				carry_bit = m_vector[current_uint] >> 63;
				// Remove the slice bits
				m_vector[current_uint] &= mask;
				// Recompose the vector
				m_vector[current_uint] |= shifted_slice;

				// increment
				if (current_uint+1 == num_uint)
					current_uint = 0;
				else
					current_uint += 1;
				continue;
			}

			// - Shift a bitvector uint that is not in the extremities of the shifted area -
			uint64_t new_value {carry_bit | m_vector[current_uint] << 1};
			carry_bit = m_vector[current_uint] >> 63;
			m_vector[current_uint] = new_value;

			// increment
			if (current_uint+1 == num_uint)
				current_uint = 0;
			else
				current_uint += 1;
		}

		// - shifts in the last uint -
		// Create mask to ignore most significant bits after to
		const uint64_t mask {(~static_cast<uint64_t>(0)) >> (63 - (to % 64))};
		// extract and shift the slice
		const uint64_t shifted_slice {(m_vector[current_uint] << 1) & mask};
		// Remove the slice bits
		m_vector[current_uint] &= ~mask;
		// Recompose the vector
		m_vector[current_uint] |= shifted_slice | carry_bit;
	}
};


#endif