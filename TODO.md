
* Add the Bzip2 dependancy in the CMake
* In Skmer header, Check how "pair& operator|= (const kuint& value)" behaves related to "pair(kuint& single) : m_value(0, single)"
* when adding a single kuint, should go to least significant not most.