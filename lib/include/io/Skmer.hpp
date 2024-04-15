#include <cstdint>
#include <ostream>
#include <assert.h>

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
    struct pair;

    pair m_pair;
    uint16_t m_pref_size;
    uint16_t m_suff_size;

    Skmer() : m_pair(), m_pref_size(0), m_suff_size(0)
    {}
    Skmer(const pair& value) : m_pair(value), m_pref_size(0), m_suff_size(0)
    {}
    Skmer(const pair& value, uint16_t prefix_size, uint16_t suffix_size) : m_pair(value), m_pref_size(prefix_size), m_suff_size(suffix_size)
    {}
    Skmer(Skmer& other) : m_pair(other.m_pair), m_pref_size(other.m_pref_size), m_suff_size(other.m_suff_size)
    {}
    Skmer(Skmer&& other) : m_pair(other.m_pair), m_pref_size(other.m_pref_size), m_suff_size(other.m_suff_size)
    {}

    Skmer<kuint>& operator= (const Skmer<kuint>& other)
    { m_pair = other.m_pair; return *this; }

    Skmer<kuint>& operator= (const pair& value)
    { m_pair = value; return *this; }

    Skmer<kuint>& operator= (Skmer<kuint>&& other)
    { m_pair = other.m_pair; return *this; }

    bool operator<(const Skmer<kuint>& other) const
    { 
        return m_pair < other.m_pair;
    }

    bool operator<=(const Skmer<kuint>& other) const
    { 
        return m_pair <= other.m_pair;
    }

    friend std::ostream& operator<<(std::ostream& os, const Skmer<kuint>& p)
    {
        os << p.m_pair << " pref:" << p.m_pref_size << " suff:" << p.m_suff_size;
        return os;
    }

    /** Neasted struct to manage pair of uints
     **/
    struct pair
    {
        // uint 0 is the less significant
        kuint m_value[2];

        pair() : m_value(0, 0)
        {}
        pair(kuint& single) : m_value(0, single)
        {}
        pair(const kuint* values) :  m_value(values[1], values[0])
        {}
        pair(const kuint& less_significant, const kuint& most_significant) : m_value(less_significant, most_significant)
        {}
        pair(const pair& other) : m_value(other.m_value[0], other.m_value[1])
        {}


        pair& operator= (const pair& other)
        {
            m_value[0] = other.m_value[0];
            m_value[1] = other.m_value[1];

            return *this;
        }

        bool operator<(const pair& other) const
        { 
            if (m_value[1] == other.m_value[1])
                return m_value[0] < other.m_value[0];
            else
                return m_value[1] < other.m_value[1];
        }

        bool operator==(const pair& other) const
        { return m_value[0] == other.m_value[0] and m_value[1] == other.m_value[1]; }

        bool operator<=(const pair& other) const
        {
            if (m_value[1] == other.m_value[1])
                return m_value[0] <= other.m_value[0];
            else
                return m_value[1] < other.m_value[1];
        }

        pair operator~ () const
        {
            return pair(~m_value[0], ~m_value[1]);
        }

        pair& operator>>= (const uint64_t shift)
        {
            if (shift >= sizeof(kuint) * 8)
            {
                m_value[0] = static_cast<kuint>(m_value[1] >> (shift - sizeof(kuint) * 8));
                m_value[1] = static_cast<kuint>(0);
            }
            else
            {
                // Most significant kuint
                const kuint transfer_mask {static_cast<kuint>((~static_cast<kuint>(0)) >> (sizeof(kuint) * 8 - shift))};
                const kuint transfer_slice {static_cast<kuint>(m_value[1] & transfer_mask)};
                m_value[1] >>= shift;
                
                // Less significant kuint
                const uint64_t shift_transfered {sizeof(kuint) * 8 - shift};
                m_value[0] >>= shift;
                m_value[0] |= transfer_slice << shift_transfered;
            }

            return *this;
        }

        pair operator>> (const uint64_t shift) const
        {
            pair p {*this};
            p >>= shift;
            return p;
        }

        pair& operator<<= (const uint64_t shift)
        {
            if (shift >= sizeof(kuint) * 8)
            {
                m_value[1] = static_cast<kuint>(m_value[0] << (shift - sizeof(kuint) * 8));
                m_value[0] = static_cast<kuint>(0);
            }
            else
            {
                // Less significant kuint
                const kuint transfer_mask {static_cast<kuint>((~static_cast<kuint>(0)) << (sizeof(kuint) * 8 - shift))};
                const kuint transfer_slice {static_cast<kuint>(m_value[0] & transfer_mask)};
                m_value[0] <<= shift;
                
                // Most significant kuint
                const uint64_t shift_transfered {sizeof(kuint) * 8 - shift};
                m_value[1] <<= shift;
                m_value[1] |= transfer_slice >> shift_transfered;
            }
            return *this;
        }

        pair operator<< (const uint64_t shift) const
        {
            pair p {*this};
            p <<= shift;
            return p;
        }

        pair& operator&= (const pair& other)
        {
            m_value[0] &= other.m_value[0];
            m_value[1] &= other.m_value[1];
            return *this;
        }

        pair operator& (const uint64_t value)
        {
            pair p{*this};
            p.m_value[0] &= value;
            p.m_value[1] = 0;
            return p;
        }

        pair operator| (const pair& other) const
        {
            pair p{*this};
            p.m_value[0] |= other.m_value[0];
            p.m_value[1] |= other.m_value[1];
            return p;
        }

        pair& operator|= (const pair& other)
        {
            m_value[0] |= other.m_value[0];
            m_value[1] |= other.m_value[1];
            return *this;
        }

        pair& operator|= (const kuint& value)
        {
            m_value[0] |= value;
            return *this;
        }


        operator uint64_t()
        {
            return m_value[0];
        }

        
        friend std::ostream& operator<<(std::ostream& os, const typename Skmer<kuint>::pair& p)
        {
            // [kuint 1] Prints the bits one at a time from the most significant to the less significant
            for (size_t idx{sizeof(kuint) * 8} ; idx>0 ; idx--)
                os << ((static_cast<uint64_t>(p.m_value[1] >>  (idx - 1))) & 1);

            os << " ";

            // [kuint 2] Prints the bits one at a time from the most significant to the less significant
            for (size_t idx{sizeof(kuint) * 8} ; idx>0 ; idx--)
                os << ((static_cast<uint64_t>(p.m_value[0] >> (idx - 1))) & 1);

            return os;
        }

    };
};



template<typename kuint>
class SkmerPrettyPrinter
{
public:
    Skmer<kuint>* m_skmer;
    uint64_t k;
    uint64_t m;
    uint64_t sk_size;
    uint64_t m_suff_size;
    uint64_t m_pref_size;

    SkmerPrettyPrinter(uint64_t k, uint64_t m): k(k), m(m)
                    , sk_size(2*k-m), m_suff_size(sk_size / 2), m_pref_size((sk_size+1) / 2)
    {};

    SkmerPrettyPrinter& operator<< (Skmer<kuint>* skmer)
    {
        m_skmer = skmer;
        return *this;
    }

};

template<typename kuint>
std::ostream& operator<<(std::ostream& os, const SkmerPrettyPrinter<kuint> pp)
{
    static const char nucleotides[] = {'A', 'C', 'T', 'G'};
    os << "[skmer not interleaved: ";

    // Forward prefix
    for (uint64_t pref_idx{0} ; pref_idx<pp.m_pref_size ; pref_idx++)
    {
        os << nucleotides[((*pp.m_skmer).m_pair >> (4 * pref_idx)) & 0b11UL];
    }
    os << " ";
    // Forward suffix
    for (uint64_t suf_idx{pp.m_suff_size} ; suf_idx>0 ; suf_idx--)
    {
        os << nucleotides[((*pp.m_skmer).m_pair >> (4 * suf_idx - 2)) & 0b11UL];
    }

    os << "]";

    return os;
}



template<typename kuint>
class SkmerManipulator
{
public:
    using kpair = typename km::Skmer<kuint>::pair;

    const uint64_t k;
    const uint64_t m;
    const uint64_t sk_size;
    
    Skmer<kuint> m_fwd;
    Skmer<kuint> m_rev;

protected:
    uint64_t m_suff_size;
    uint64_t m_pref_size;

    Skmer<kuint>::pair m_fwd_suffix_buff;
    Skmer<kuint>::pair m_fwd_prefix_buff;
    Skmer<kuint>::pair m_rev_suffix_buff;
    Skmer<kuint>::pair m_rev_prefix_buff;

    const Skmer<kuint>::pair max_pair_value;
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
        m_fwd_suffix_buff = {0, 0};
        m_fwd_prefix_buff = {0, 0};
        m_rev_suffix_buff = {0, 0};
        m_rev_prefix_buff = {0, 0};

        // Skmers
        m_fwd = Skmer<kuint>{};
        m_rev = Skmer<kuint>{};
    }

    /** Add a binarized nucleotide (2bits) to the current skmer.
     * @param nucl a 2-bits binarized nucleotide according the current encoding. Warning: The nucleotide is not checked before insertion, so it can destroy the skmer if not correctly formated.
     * @return The current canonical skmer according to the interleaved order.
     **/
    inline Skmer<kuint>& add_nucleotide(const kuint nucl)
    {
        // --- forward prefix ---
        // Shift prefix to the right
        m_fwd_prefix_buff >>= 4;
        // Get nucleotide that move from suffix to prefix
        const auto fwd_central_nucl {m_fwd_suffix_buff >> (m_suff_size * 4 - 2)};
        // Include the nucleotide in the prefix
        m_fwd_prefix_buff |= fwd_central_nucl << ((m_pref_size-1) * 4);

        // --- reverse suffix ---
        // Shift the suffix to the right
        m_rev_suffix_buff >>= 4;
        // Get the nucleotide to transfer from the prefix to suffix
        const auto rev_central_nucl {m_rev_prefix_buff >> ((m_pref_size - 1) * 4)};
        // Include the transfered nucleotide
        m_rev_suffix_buff |= rev_central_nucl << (m_suff_size * 4 - 2);

        // --- forward suffix ---
        // Shift the suffix
        m_fwd_suffix_buff <<= 4;
        // Add the new nucleotide
        m_fwd_suffix_buff |= nucl << 2;
        // Remove the transfered nucleotide
        m_fwd_suffix_buff &= m_mask;

        // --- reverse prefix ---
        // Shift the prefix
        m_rev_prefix_buff <<= 4;
        // Add the new complement nucleotide
        const auto compl_nucl {static_cast<kuint>((nucl + 2) % 4)};
        m_rev_prefix_buff |= compl_nucl;
        // Remove the transfered nucleotide
        m_rev_prefix_buff &= m_mask;

        // --- Merge the interleaved halves ---
        m_fwd = m_fwd_prefix_buff | m_fwd_suffix_buff;
        m_rev = m_rev_prefix_buff | m_rev_suffix_buff;
        
        if (m_rev < m_fwd)
            return m_rev;
        else
            return m_fwd;
    }

    kuint minimizer() const {
        return std::min(
            static_cast<kuint>(m_fwd.m_pair >> (4*(k-m))),
            static_cast<kuint>(m_rev.m_pair >> (4*(k-m)))
        );
    }

    kuint minimizer(const Skmer<kuint> skmer) const {
        return std::min(
            static_cast<kuint>(skmer.m_fwd.m_pair >> (4*(k-m))),
            static_cast<kuint>(skmer.m_rev.m_pair >> (4*(k-m)))
        );
    }

    template<typename T>
    friend std::ostream& operator<<(std::ostream& os, SkmerManipulator<T>& manip);
};


template<typename T>
std::ostream& operator<<(std::ostream& os, SkmerManipulator<T>& manip)
{
    static const char nucleotides[] = {'A', 'C', 'T', 'G'};
    
    // cout << manip.m_fwd_suffix_buff << endl;
    // cout << manip.m_fwd_prefix_buff << endl;
    
    os << "[not interleaved: ";

    // Forward prefix
    for (uint64_t pref_idx{0} ; pref_idx<manip.m_pref_size ; pref_idx++)
    {
        os << (nucleotides[(manip.m_fwd_prefix_buff >> (4 * pref_idx)) & 0b11UL]);
    }
    os << " ";
    // Forward suffix
    for (uint64_t suf_idx{manip.m_suff_size} ; suf_idx>0 ; suf_idx--)
    {
        os << (nucleotides[(manip.m_fwd_suffix_buff >> (4 * suf_idx - 2)) & 0b11UL]);
    }

    os << " / ";

    // Reverse prefix
    for (uint64_t pref_idx{0} ; pref_idx<manip.m_pref_size ; pref_idx++)
    {
        os << nucleotides[(manip.m_rev_prefix_buff >> (4 * pref_idx)) & 0b11UL];
    }
    os << " ";
    // Reverse suffix
    for (uint64_t suf_idx{manip.m_suff_size} ; suf_idx>0 ; suf_idx--)
    {
        os << nucleotides[(manip.m_rev_suffix_buff >> (4 * suf_idx - 2)) & 0b11UL];
    }

    os << "]";

    return os;
}

};


#endif