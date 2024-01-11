
#ifndef SKMER_H
#define SKMER_H


template<typename kuint>
class Skmer
{
protected:
    // kuint 1 is the most significant uint
    kuint m_value[2];

public:
    Skmer() : m_value({0, 0})
    {}
    Skmer(kuint value[2]) : m_value({value[0], value[1]})
    {}
    Skmer(Skmer& other) : m_value({other.value[0], other.value[1]})
    {}
    Skmer(Skmer&& other) : m_value({other.value[0], other.value[1]})
    {}

    Skmer<kuint>& operator= (Skmer<kuint>& other)
    { m_value[0] = other.m_value[0]; m_value[1] = other.m_value[1]; return *this; }

    Kmer<kuint>& operator= (Kmer<kuint>&& other)
    { m_value[0] = other.m_value[0]; m_value[1] = other.m_value[1]; return *this; }

    bool operator<(const Skmer<kuint>& other) const
    { 
        if (m_value[1] == other.m_value[1])
            return m_value[0] < other.m_value[0];
        else
            return m_value[1] < other.m_value[1];
    }

    bool operator==(const Skmer<kuint>& other) const
    { return m_value[0] == other.m_value[0] and m_value[1] == other.m_value[1]; }

};


template<typename kuint>
class SkmerManipulator
{
public:
    const uint64_t k;
    const uint64_t m;
    const uint64_t sk_size;
protected:
    Skmer<kuint> m_fwd;
    Skmer<kuint> m_rev;
    
    kuint m_suffix_buff[2];
    kuint m_prefix_buff[2];

    const kuint m_mask[2];

public:
    KmerManipulator(const uint64_t k, const uint64_t m) 
        : k(k), m(m), sk_size(2*k-m)
    {
        assert((k*2-m+3) / 4 < 2*sizeof(kuint));
        
        // mask creation
        m_mask[0] = (~static_cast<kuint>(0));
        // All the skmer in the lower kuint ?
        if ((sk_size+3)/4 < sizeof(kuint))
        {
            m_mask[0] >>= sizeof(kuint) * 8 - 2 * sk_size;
        }

        m_mask[1] = (~static_cast<kuint>(0));
        const uint64_t left_kuint_shift {2 * sizeof(kuint) * 8 - 2 * sk_size};
        m_mask >>= left_kuint_shift;

        // Skmer and skmer buffers init
        this->init_skmer();
    }

    void init_skmer()
    {
        // Buffers
        m_suffix_buff = {0, 0};
        m_prefix_buff = {0, 0};

        // Skmers
        m_fwd = Skmer{};
        m_rev = Skmer{};
    }

    inline Skmer<kuint> add_nucleotide(const kuint nucl)
    {
        
    }

    template<typename T>
    friend std::ostream& operator<<(std::ostream& os, SkmerManipulator<T>& manip);
};


template<typename T>
std::ostream& operator<<(std::ostream& os, SkmerManipulator<T>& manip)
{
    static const char nucleotides[] = {'A', 'C', 'T', 'G'};
    os << '[';

    // Forward
    for (uint64_t idx{0} ; idx<manip.k ; idx++)
        os << nucleotides[(manip.m_fwd.m_value >> (2 * idx)) & 0b11U];
    os << '(' << manip.m_fwd.m_value << ") / ";

    // Reverse
    for (uint64_t idx{0} ; idx<manip.k ; idx++)
        os << nucleotides[(manip.m_rev.m_value >> (2 * idx)) & 0b11U];
    os << '('  << manip.m_rev.m_value << ")]";

    return os;
}


#endif