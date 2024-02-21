#include <cstdint>
#include <array>


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

	/** Shift the bitvector 1 bit to the right. from and to are included positions. Left bit is 0.
	 * Right bit overflowing is discarded.
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