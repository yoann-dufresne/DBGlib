
#ifndef SKMER_H
#define SKMER_H


using namespace std;


namespace km
{


/** This superkmer class represents a full size superkmer (ie 2*k-m nucleotides). It is stored in a interleavec way:
 * It means the the most significant nucleotide in the middle one in the superkmer. For example if you have a superkmer
 * ABCDE, its representation here will be CDBEA. We made this choice as the more a nucleotide is "central" in the 
 * superkmer, the most it is shared by the kmers from the superkmers.
 **/
template<typename kuint>
class Skmer
{

public:
    template<typename I>
    struct pair;

    pair<kuint> m_pair;

    Skmer() : m_pair()
    {}
    Skmer(const pair<kuint>& value) : m_pair(value)
    {}
    Skmer(Skmer& other) : m_pair(other.m_pair)
    {}
    Skmer(Skmer&& other) : m_pair(other.m_pair)
    {}

    Skmer<kuint>& operator= (const Skmer<kuint>& other)
    { m_pair = other.m_pair; return *this; }

    Skmer<kuint>& operator= (const pair<kuint>& value)
    { m_pair = value; return *this; }

    Skmer<kuint>& operator= (Skmer<kuint>&& other)
    { m_pair = other.m_pair; return *this; }

    /** Neasted struct to manage pair of uints
     **/
    template<typename I>
    struct pair
    {
        // uint 0 is the less significant
        I m_value[2];

        pair() : m_value(0, 0)
        {}
        pair(I& single) : m_value(0, single)
        {}
        pair(const I* values) : m_value(values)
        {}
        pair(const I& less_significant, const I& most_significant) : m_value(less_significant, most_significant)
        {}
        pair(const pair<I>& other) : m_value(other.m_value[0], other.m_value[1])
        {}


        pair<I>& operator= (const pair<I>& other)
        {
            m_value[0] = other.m_value[0];
            m_value[1] = other.m_value[1];

            return *this;
        }

        bool operator<(const pair<I>& other) const
        { 
            if (m_value[1] == other.m_value[1])
                return m_value[0] < other.m_value[0];
            else
                return m_value[1] < other.m_value[1];
        }

        bool operator==(const pair<I>& other) const
        { return m_value[0] == other.m_value[0] and m_value[1] == other.m_value[1]; }

        pair<I> operator~ () const
        {
            return pair(~m_value[0], ~m_value[1]);
        }

        pair<I>& operator>>= (const uint64_t shift)
        {
            if (shift >= sizeof(I) * 8)
            {
                m_value[0] = static_cast<I>(m_value[1] >> (shift - sizeof(I) * 8));
                m_value[1] = static_cast<I>(0);
            }
            else
            {
                // Most significant kuint
                const I transfer_mask {static_cast<I>((~static_cast<I>(0)) >> (sizeof(I) * 8 - shift))};
                const I transfer_slice {static_cast<I>(m_value[1] & transfer_mask)};
                m_value[1] >>= shift;
                
                // Less significant kuint
                const uint64_t shift_transfered {sizeof(I) * 8 - shift};
                m_value[0] >>= shift;
                m_value[0] |= transfer_slice << shift_transfered;
            }

            return *this;
        }

        pair<I> operator>> (const uint64_t shift) const
        {
            pair<I> p {*this};
            p >>= shift;
            return p;
        }

        pair<I>& operator<<= (const uint64_t shift)
        {
            if (shift >= sizeof(I) * 8)
            {
                m_value[1] = static_cast<I>(m_value[0] << (shift - sizeof(I) * 8));
                m_value[0] = static_cast<I>(0);
            }
            else
            {
                // Less significant kuint
                const I transfer_mask {static_cast<I>((~static_cast<I>(0)) << (sizeof(I) * 8 - shift))};
                const I transfer_slice {static_cast<I>(m_value[0] & transfer_mask)};
                m_value[0] <<= shift;
                
                // Most significant kuint
                const uint64_t shift_transfered {sizeof(I) * 8 - shift};
                m_value[1] <<= shift;
                m_value[1] |= transfer_slice >> shift_transfered;
            }
            return *this;
        }

        pair<I> operator<< (const uint64_t shift) const
        {
            pair<I> p {*this};
            p <<= shift;
            return p;
        }

        pair<I>& operator&= (const pair<I>& other)
        {
            m_value[0] &= other.m_value[0];
            m_value[1] &= other.m_value[1];
            return *this;
        }

        pair<I> operator| (const pair<I>& other) const
        {
            pair<I> p{*this};
            p.m_value[0] |= other.m_value[0];
            p.m_value[1] |= other.m_value[1];
            return p;
        }

        pair<I>& operator|= (const pair<I>& other)
        {
            m_value[0] |= other.m_value[0];
            m_value[1] |= other.m_value[1];
            return *this;
        }

        pair<I>& operator|= (const I& value)
        {
            m_value[0] |= value;
            return *this;
        }

        
        friend std::ostream& operator<<(std::ostream& os, const typename Skmer<kuint>::pair<kuint>& p)
        {
            os << static_cast<uint64_t>(p.m_value[1]) << " " << static_cast<uint64_t>(p.m_value[0]);
            return os;
        }

    };
};



template<typename kuint>
class SkmerManipulator
{
public:
    using kpair = typename km::Skmer<kuint>::template pair<kuint>;

    const uint64_t k;
    const uint64_t m;
    const uint64_t sk_size;
    
    Skmer<kuint> m_fwd;
    Skmer<kuint> m_rev;

protected:
    uint64_t m_suff_size;
    uint64_t m_pref_size;

    Skmer<kuint>::template pair<kuint> m_suffix_buff;
    Skmer<kuint>::template pair<kuint> m_prefix_buff;

    const Skmer<kuint>::template pair<kuint> max_pair_value;
    const kpair m_mask;

    // // The amount of bit shifts needed to reach the 4 most significant bits of a kuint
    // static constexpr uint64_t uints_middle_shift {sizeof(kuint) * 8 - 4};

public:
    SkmerManipulator(const uint64_t k, const uint64_t m) 
        : k(k), m(m), sk_size(2*k-m), m_suff_size(sk_size / 2), m_pref_size((sk_size+1) / 2)
        , max_pair_value(static_cast<kuint>(~static_cast<kuint>(0)), static_cast<kuint>(~static_cast<kuint>(0)))
        , m_mask( max_pair_value >> (2 * sizeof(kuint) * 8 - 2 * sk_size) )
    {
        assert((k*2-m+3) / 4 <= 2*sizeof(kuint));

        // Skmer and skmer buffers init
        this->init_skmer();
    }

    void init_skmer()
    {
        // Buffers
        m_suffix_buff = {0, 0};
        m_prefix_buff = {0, 0};

        // Skmers
        m_fwd = Skmer<kuint>{};
        m_rev = Skmer<kuint>{};
    }

    inline Skmer<kuint>& add_nucleotide(const kuint nucl)
    {
        // --- prefix ---
        // Shift prefix to the right
        m_prefix_buff >>= 4;
        // Get nucleotide that move from suffix to prefix
        const auto central_nucl {m_suffix_buff >> (m_suff_size * 4 - 2)};
        // Include the nucleotide in the prefix
        m_prefix_buff |= central_nucl << ((m_pref_size-1) * 4);

        // --- suffix ---
        // Shift the suffix
        m_suffix_buff <<= 4;
        // Add the new nucleotide
        m_suffix_buff |= nucl << 2;
        // Remove the transfered nucleotide
        m_suffix_buff &= m_mask;

        // Merge the interleaved halves
        m_fwd = m_prefix_buff | m_suffix_buff;

        return m_fwd;
    }

    // template<typename T>
    // friend std::ostream& operator<<(std::ostream& os, SkmerManipulator<T>& manip);
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


};


#endif