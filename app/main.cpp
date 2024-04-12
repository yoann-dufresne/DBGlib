#include <iostream>
#include <string>
#include <cstdint>
#include <io/Skmer.hpp>


using namespace std;
using namespace km;


int main([[maybe_unused]]int argc, [[maybe_unused]]char const *argv[])
{
	cout << endl;

	const string seq {"CCCAACCCAACCCCCACC"};
	const uint64_t k {3};
	const uint64_t m {2};

	SkmerManipulator<uint8_t> manip{k, m};
	cout << manip << endl;

	uint64_t idx {0};
	for (const char c : seq)
	{
		cout << c << endl;
		manip.add_nucleotide((c >> 1) & 0b11);
		cout << manip << endl;

		cout << endl;
		idx += 1;
		if (idx > 2)
			break;
	}

	return 0;
}