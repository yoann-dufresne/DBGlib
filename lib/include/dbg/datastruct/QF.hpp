#include <array>
#include <algorithm>


// typedef unsigned __int128 uint128_t;


template<uint64_t slot_size>
struct RestBlock
{
	std::array<uint64_t, slot_size> rests{};
	
	RestBlock(){};

	uint64_t get(size_t index)
	{
		// Get the position of the value in bits
		const auto first_bit_position { index * slot_size };
		const auto first_uint_position { first_bit_position * slot_size };
		const auto bits_in_uint { std::min((64UL - (first_bit_position % 64)), slot_size) };
		const bool second_byte { bits_in_uint >= slot_size };

		// Get the first uint
		auto value { rests[first_uint_position] };

		// Shift right if only one uint
		if (not second_byte)
		{
			const auto right_shift { 64UL - slot_size - first_bit_position };
			value >>= right_shift;
		}
		// Shift left and add the second uint if needed
		else
		{
			const auto second_bits { slot_size - bits_in_uint };
			// Shift left to allow insertion
			value <<= second_bits;
			// Mask to remove right bits
			const auto left_mask { (1UL << second_bits) - 1 };
			const auto right_mask { ~left_mask };
			value &= right_mask;

			// Get the second uint
			auto right_value { rests[first_uint_position+1] };
			// Shift right to align
			right_value >>= 64UL - second_bits;
			// Mask to remove the left bits
			right_value &= left_mask;

			// Compose the final value from the two parts
			value |= right_value;
		}

		// Mask to remove left bits
		constexpr uint64_t mask { (1UL << slot_size) - 1UL };
		value &= mask;

		return value;
	};
};