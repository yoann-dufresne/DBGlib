#include <array>
#include <algorithm>


// #include <iostream>
// using namespace std;


/** Contains a packed array of 64 values of size slot_size. This is a building block for packed hash tables, counting bloom filters and quotient filter.
 **/
template<uint64_t slot_size>
struct PackedBlock
{
	std::array<uint64_t, slot_size> rests{};
	
	PackedBlock(){};

	/** Get the value at index (converted in uint64_t).
	 **/
	uint64_t get(size_t index)
	{
		// cout << "get " << index << endl;
		// Get the position of the value in bits
		const auto first_bit_position { index * slot_size };
		const auto first_uint_position { first_bit_position / 64 };
		const auto bits_in_uint { std::min((64UL - (first_bit_position % 64)), slot_size) };
		const bool second_byte { bits_in_uint < slot_size };

		// cout << first_bit_position << " " << first_uint_position << endl;
		// cout << first_bit_position << "/" << slot_size << " = " << first_uint_position << endl;
		// cout << bits_in_uint << " " << second_byte << endl;

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

		// cout << "/get" << endl;
		return value;
	};


	/** Set the slot_size lower bits of the value to the index part of the array
	 * @param index Where to insert
	 * @param value Value to insert (slot_size lowest bits only)
	 **/
	void set(size_t index, uint64_t value)
	{
		// cout << "set " << index << endl;
		// Get the position of the value in bits
		const auto first_bit_position { index * slot_size };
		const auto first_uint_position { first_bit_position / 64 };
		const auto bits_in_uint { std::min((64UL - (first_bit_position % 64)), slot_size) };
		const bool second_byte { bits_in_uint < slot_size };

		// cout << first_bit_position << " " << first_uint_position << endl;
		// cout << first_bit_position << "/" << slot_size << " = " << first_uint_position << endl;
		// cout << bits_in_uint << " " << second_byte << endl;

		if (not second_byte)
		{
			// Align the value
			// Mask already present bits
			// Insert the value
		}
		else
		{
			// --- first uint ---
			// Right align the higher bits of the value
			// Mask the right bits of the first uint
			// Insert the high bits

			// --- second uint ---
			// Left align the lower bits of the value
			// Mask the left bits of the second uint
			// Insert the low bits
		}

		// cout << "/set" << endl;
	};
};