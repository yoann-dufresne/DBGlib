#include <array>


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
		const auto bits_in_uint { std::min((64ULL - (first_bit_position % 64)), slot_size) };
		const bool second_byte { bits_in_uint >= slot_size };

		// Get the first uint
		auto value { rests[first_uint_position] };

		// Shift right if only one uint
		if (second_byte)
		{
			const auto right_shift { 64ULL - slot_size - first_bit_position };
			value >>= right_shift;
		}
		// Shift left and add the second uint if needed
		else
		{
			const auto second_bits { slot_size - bits_in_uint };
			// Shift left to allow insertion
			// Mask to remove right bits

			// Get the second uint
			auto right_value { rests[first_uint_position+1] };
			// Shift right to align
			// Mask to remove the left bits

			// Compose the final value from the two parts
			value |= right_value;
		}

		// Mask to remove left bits
		constexpr uint64_t mask { (1ULL << slot_size) - 1ULL };
		value &= mask;

		return value;
	};
};