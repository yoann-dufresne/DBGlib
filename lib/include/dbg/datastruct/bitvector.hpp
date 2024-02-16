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

	constexpr uint64_t m_num_uint {size / 64};
	
public:
	std::array<uint64_t, (size+63UL)/64> m_vector {};

	Bitvector () {};

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
		const uint64_t first_uint {from/64UL};
		const uint64_t last_uint {to/64UL};
		uint64_t current_uint {(from + 63) / 64};

		// If everything is in the same uint
		if (first_uint == last_uint)
		{
			// TODO
			return;
		}

		uint64_t saved_bit {0};
		while (current_uint != last_uint)
		{
			// Mask the beginning if this is the first uint to be shifted
			if (first_uint)
			{
				// Create mask to ignore less significant bits until from
				uint64_t mask;
				// TODO

				// Modify the vector value
				uint64_t new_value {m_vector[current_uint] << 1};
				saved_bit = m_vector[current_uint] >> 63;
				m_vector[current_uint] = new_value;
				continue;
			}

			// Shift the uint of the bitvector
			uint64_t new_value {saved_bit | m_vector[current_uint] << 1};
			saved_bit = m_vector[current_uint] >> 63;
			m_vector[current_uint] = new_value;

			// increment
			if (current_uint == m_num_uint)
				current_uint = 0;
			else
				current_uint += 1;
		}

		// shifts in the last uint
	}
};


#endif