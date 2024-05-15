#include <iostream>
#include <string>
#include <memory>
#include <kseq++/seqio.hpp>

#include <io/Skmer.hpp>


#ifndef SKMERATOR_H
#define SKMERATOR_H



namespace km
{


template <typename kuint>
class FileSkmerator;



// ----------------------- NEW SEQ ITERATOR ---------------------------

template <typename kuint>
class SeqSkmerator
{
protected:
    std::string empty_str{};
    
    SkmerManipulator<kuint>& m_manip;
    std::string& m_seq;
    Skmer<kuint> m_yielded_skmer;

public:

    SeqSkmerator(SkmerManipulator<kuint>& skmer_manipulator, std::string& sequence)
        : m_manip(skmer_manipulator), m_seq(sequence)
    {};

    SeqSkmerator(SkmerManipulator<kuint>& skmer_manipulator) 
        : SeqSkmerator(skmer_manipulator, empty_str)
    {};

    SeqSkmerator& operator=(const SeqSkmerator& other)
    {
        m_seq = other.m_seq;
        m_yielded_skmer = other.m_yielded_skmer;

        return *this;
    }

    Skmer<kuint>& current_skmer()
    {
        return m_yielded_skmer;
    }

    struct Iterator
    {

    private:
        std::string empty_str{};
        // Sequence file related attributes
        SeqSkmerator<kuint>& m_rator;
        std::string& m_seq;
        int64_t m_remaining_nucleotides;
        bool m_consumed;

        // Manipulator to compute interleaved along the sequence
        SkmerManipulator<kuint>& m_manip;
        kuint m_current_minimizer;

        // Buffered unmasked skmers
        uint64_t m_buffer_size;
        Skmer<kuint>* m_skmer_buffer_array;
        uint64_t m_ptr_current;
        uint64_t m_ptr_min;
        uint64_t m_ptr_last_round;

    public:
        ~Iterator()
        {
            delete[] m_skmer_buffer_array;
        }

        bool consumed() const
        {
            return m_consumed;
        }

    protected:
        // Construct an iterator without control on the file stream
        Iterator(SeqSkmerator& skmerator, std::string& sequence)
            : m_rator(skmerator), m_seq(sequence), m_remaining_nucleotides(sequence.length())
            , m_consumed(false)
            , m_manip(skmerator.m_manip), m_current_minimizer(~static_cast<kuint>(0))
            , m_buffer_size(2 * skmerator.m_manip.k - skmerator.m_manip.m)
            , m_skmer_buffer_array(new Skmer<kuint>[m_buffer_size])
            , m_ptr_current(skmerator.m_manip.k - skmerator.m_manip.m)
            , m_ptr_min(0) // minimizer_position
            , m_ptr_last_round(0)
        {
            if (m_remaining_nucleotides < static_cast<int64_t>(m_manip.k))
            {
                m_consumed = true;
                return;
            }

            this->init_seq();
            this->operator++();
        }

        Iterator(SeqSkmerator& skmerator)
            : Iterator(skmerator, empty_str)

        {};

        Iterator& operator=(Iterator& other)
        {
            m_rator = other.m_rator;
            m_seq = other.m_seq;
            m_remaining_nucleotides = other.m_remaining_nucleotides;
            m_consumed = other.m_consumed;
            m_manip = other.m_manip;
            m_current_minimizer = other.m_current_minimizer;
            
            m_buffer_size = other.m_buffer_size;
            delete[] m_skmer_buffer_array;
            m_skmer_buffer_array = new Skmer<kuint>[m_buffer_size];
            for (uint64_t idx{0} ; idx<m_buffer_size ; idx++)
                m_skmer_buffer_array [idx] = other.m_skmer_buffer_array[idx];

            m_ptr_current = other.m_ptr_current;
            m_ptr_min = other.m_ptr_min;
            m_ptr_last_round = other.m_ptr_last_round;

            return *this;
        }

        void init_seq()
        {
            // Init the first kmer
            for (uint64_t idx{0} ; idx<m_manip.k-1 ; idx++)
            {
                // Nucl encoding. TODO: Move encoding to dedicated classes
                const kuint nucl {static_cast<kuint>((m_seq[idx] >> 1) & 0b11U)};
                m_manip.add_nucleotide(nucl);
            }

            m_current_minimizer = ~static_cast<kuint>(0);

            m_ptr_current = m_rator.m_manip.k - 2;
            m_ptr_min = m_rator.m_manip.k - 2;
            m_remaining_nucleotides -= m_manip.k - 1;
        }

        friend class SeqSkmerator;
        friend class FileSkmerator<kuint>;

    public:
        // Return kmer by value
        Skmer<kuint> operator*() const
        {
            // cout << "operator*" << endl;
            return m_rator.m_yielded_skmer;
        }

        Iterator& operator++()
        {
            const uint64_t k {m_manip.k};
            const uint64_t m {m_manip.m};


            // End of the sequence => final yieldings
            if (m_remaining_nucleotides + k - m == 0)
            {
                // Yield the stored but not returned skmers while sequence is already over
                do
                {
                    m_ptr_last_round += 1;
                    // Get yielding candidate
                    Skmer<kuint>& skmer {m_skmer_buffer_array[m_ptr_last_round % m_buffer_size]};

                    // Yield if needed
                    if (skmer.m_pref_size + skmer.m_suff_size >= k - m)
                    {
                        m_rator.m_yielded_skmer = skmer;
                        m_manip.mask_absent_nucleotides(m_rator.m_yielded_skmer);
                        return *this;
                    }
                }
                while ((m_ptr_last_round % m_buffer_size) != (m_ptr_current % m_buffer_size));

                m_consumed = true;
                return *this;
            }

            while (m_remaining_nucleotides + k - m > 0)
            {
                // -- Save the skmer to eventually yield
                m_rator.m_yielded_skmer = m_skmer_buffer_array[(m_ptr_current + 1) % m_buffer_size];

                // -- On out of context minimizer
                if (m_ptr_current - m_ptr_min >= k - m)
                {
                    // Set the suffix size of the ooc skmer
                    m_skmer_buffer_array[m_ptr_min % m_buffer_size].m_suff_size = k - m;
                    // TODO: recompute the previous minimizer
                    this->recompute_minimizer(m_ptr_min + 1, m_ptr_current);
                }
                

                // -- Compute the new candidate skmer
                m_remaining_nucleotides -= 1;
                m_ptr_current += 1;

                // Get the next skmer 
                // If we reach the end of the sequence we add fake nucleotides (0b11) to complete the last skmer  
                const kuint nucl {
                    m_remaining_nucleotides >= 0 ?(static_cast<kuint>((m_seq[m_ptr_current] >> 1) & 0b11U)) : static_cast<kuint>(0b11U)};
                
                // add nucleotide to the current candidate superkmer
                m_skmer_buffer_array[ m_ptr_current % m_buffer_size ] = m_manip.add_nucleotide(nucl);
                Skmer<kuint>& candidate {m_skmer_buffer_array[ m_ptr_current % m_buffer_size ]};
                
                // Get the minimizer
                const kuint candidate_minimizer {m_manip.minimizer(candidate)};

                // -- A new minimizer has been discovered
                if (candidate_minimizer < m_current_minimizer) {
                    // 1 - Prefix the new skmer
                    candidate.m_pref_size = k - m;

                    // 2 - define the suffix of the previous skmer that was including the minimizer
                    Skmer<kuint>& mini_skmer {m_skmer_buffer_array[m_ptr_min % m_buffer_size]};
                    mini_skmer.m_suff_size = m_ptr_current - m_ptr_min - 1;

                    // 3 - Limit the suffixes of the previous skmers
                    for (uint64_t idx{0} ; idx < (k - m) ; idx++)
                    {
                        Skmer<kuint>& predecessor {m_skmer_buffer_array[(m_ptr_current - (k - m) + idx) % m_buffer_size]};
                        // Mask the nucleotides from the predecessor that are part of this skmer.
                        predecessor.m_suff_size = std::min( static_cast<uint16_t>(predecessor.m_suff_size)
                                                          , static_cast<uint16_t>(k - m - idx - 1));

                        // If end of the sequence
                        const int64_t remaining_prev {m_remaining_nucleotides + static_cast<int64_t>(k - m - idx)};
                        if (remaining_prev < 0)
                            predecessor.m_suff_size = std::min(predecessor.m_suff_size, static_cast<uint16_t>(k - m +remaining_prev));
                    }
                    
                    // 4 - save the new current minimal skmer and minimizer
                    m_ptr_min = m_ptr_current;
                    m_current_minimizer = candidate_minimizer;
                }

                // The candidate minimizer is the same than the previous minimizer
                else if (candidate_minimizer == m_current_minimizer)
                {
                    this->update_on_equal_mini(m_ptr_current);
                }

                // Correction of the prefix size for the beginning of the sequence
                if (m_ptr_current < 2 * k - m - 1)
                {
                    uint16_t max_prefix {static_cast<uint16_t>(m_ptr_current - (k - 1))};
                    candidate.m_pref_size = std::min(candidate.m_pref_size, max_prefix);
                }

                // -- Yield if needed
                if (m_rator.m_yielded_skmer.m_pref_size + m_rator.m_yielded_skmer.m_suff_size >= k - m)
                {
                    m_manip.mask_absent_nucleotides(m_rator.m_yielded_skmer);
                    return *this;
                }

            } // End of the while over the sequence

            Skmer<kuint>& last_skmer = m_skmer_buffer_array[m_ptr_min % m_buffer_size];
            last_skmer.m_suff_size = m_ptr_current - m_ptr_min;
            
            // Recursive call to return the already computed skmer array
            m_ptr_last_round = m_ptr_current;
            return this->operator++();
        }

        void update_on_equal_mini(uint64_t ptr_equivalent)
        {
            static const uint64_t k{m_manip.k};
            static const uint64_t m{m_manip.m};

            const uint64_t pos_diff {ptr_equivalent - m_ptr_min};
            Skmer<kuint>& current_skmer {m_skmer_buffer_array[ptr_equivalent % m_buffer_size]};
            Skmer<kuint>& prev_skmer {m_skmer_buffer_array[m_ptr_min % m_buffer_size]};

            // 0 - Due to the way that we encode skmers, the no suffix kmer in the current skmer will always be with the previous skmer.
            uint64_t first_kmer_in_new_skmer {1};

            // 1 - Scan all the kmer shared between the 2 skmers to decide where is the limit between the 2 skmers
            for ( ; first_kmer_in_new_skmer < k - m - pos_diff ; first_kmer_in_new_skmer++)
            {
                const uint64_t prev_skmer_km_pos {first_kmer_in_new_skmer + pos_diff};

                // 2 - The shared kmer is part of the new skmer
                if (m_manip.kmer_lt_kmer(current_skmer, first_kmer_in_new_skmer, prev_skmer, prev_skmer_km_pos))
                    break;
            }

            const uint64_t nb_kmers_in_prev {first_kmer_in_new_skmer};

            // 3 - Update suffix of the previous skmer and prefix of the new skmer
            prev_skmer.m_suff_size = nb_kmers_in_prev;
            current_skmer.m_pref_size = k - m - nb_kmers_in_prev;

            // If end of the sequence
            const int64_t remaining_prev {m_remaining_nucleotides + static_cast<int64_t>(pos_diff)};
            if (remaining_prev < 0)
                prev_skmer.m_suff_size = std::min(prev_skmer.m_suff_size, static_cast<uint16_t>(k - m +remaining_prev));


            m_ptr_min = ptr_equivalent;
        }

        /** Compute the dominant minimizer in the slice of the buffer. If multiple identical minimizers are present
         * it will initialize all the prefixes and suffixes of skmers needed to reach the last one.
         * @param buff_start First buffer coordinate to analyse (absolute value)
         * @param buff_stop Last buffer coordinate to analyse (absolute value)
         **/
        void recompute_minimizer(uint64_t buff_start, uint64_t buff_stop)
        {
            // 0 - Select first candidate
            Skmer<kuint>& first = m_skmer_buffer_array[buff_start % m_buffer_size];
            m_current_minimizer = m_manip.minimizer(first);
            m_ptr_min = buff_start;
            uint64_t nb_identical = 1;

            // 1 - Compute the minimizer in the window
            for (uint64_t idx{buff_start+1} ; idx<=buff_stop ; idx++)
            {
                Skmer<kuint>& candidate = m_skmer_buffer_array[idx % m_buffer_size];
                kuint candidate_minimizer = m_manip.minimizer(candidate);

                if (candidate_minimizer < m_current_minimizer)
                {
                    m_current_minimizer = candidate_minimizer;
                    m_ptr_min = idx;
                    nb_identical = 1;
                }
                else if (candidate_minimizer == m_current_minimizer)
                {
                    nb_identical += 1;
                }
            }

            // 2 - For each skmer with the minimizer compute pref/suff sizes
            m_skmer_buffer_array[m_ptr_min % m_buffer_size].m_pref_size = m_ptr_min - buff_start;

            // Multiple identical minimizers
            if (nb_identical > 1)
            {
                // Iterate over the buffer to find identical minimizers
                for (uint64_t idx{m_ptr_min+1} ; idx<=buff_stop ; idx++)
                {
                    Skmer<kuint>& candidate {m_skmer_buffer_array[idx % m_buffer_size]};
                    if (m_manip.minimizer(candidate) == m_current_minimizer)
                        this->update_on_equal_mini(idx);
                }
            }
        }


        // Warning: This function suppose that we are comparing iterator over the same sequence.
        bool operator==(Iterator& it) const
        {
            if (m_consumed and it.m_consumed)
                return true;

            return m_remaining_nucleotides == it.m_remaining_nucleotides;
        }

    };

    Iterator begin() { return Iterator(*this, m_seq); }
    Iterator end() { return Iterator(*this); }
};


// ----------------------- /NEW SEQ ITERATOR ---------------------------


template <typename kuint>
class FileSkmerator
{
protected:
    std::string m_filename;
    SkmerManipulator<kuint>& m_manip;

public:
    FileSkmerator(SkmerManipulator<kuint>& manipulator, const std::string& filename)
        : m_filename(std::string{filename}), m_manip(manipulator)
    {};

    FileSkmerator<kuint>& operator=(const FileSkmerator<kuint>& other)
    {
        m_filename = other.m_filename;
        m_manip = other.m_manip;

        return *this;
    }

    struct Iterator
    {

    private:
        // Sequence file related attributes
        FileSkmerator<kuint>& m_rator;
        std::unique_ptr<klibpp::SeqStreamIn> m_ptr;
        klibpp::KSeq m_record;
        // skmer sequence enumerator
        SeqSkmerator<kuint> m_seq_rator;
        SeqSkmerator<kuint>::Iterator m_skmer_iterator;

    protected:
        // Construct an iterator without control on the file stream
        Iterator(FileSkmerator<kuint>& skmerator, std::unique_ptr<klibpp::SeqStreamIn> stream_ptr)
            : m_rator(skmerator), m_ptr(std::move(stream_ptr))
            , m_seq_rator(skmerator.m_manip), m_skmer_iterator(m_seq_rator.begin())
        {
            if (m_ptr == nullptr)
                return;

            this->init_record();
            this->operator++();
        }

        // Construct a new file stream from the filename
        Iterator(FileSkmerator<kuint>& skmerator)
            : Iterator ( skmerator, std::make_unique<klibpp::SeqStreamIn>(skmerator.m_filename.c_str()) )
        {}

        void init_record()
        {
            // cout << "init_record" << endl;
            do
            {
                if ((*m_ptr) >> m_record)
                {
                    // If sequence is too short, skip it
                    if (m_record.seq.length() < m_rator.m_manip.k) {
                        continue;
                    }

                    m_seq_rator = SeqSkmerator<kuint>(m_rator.m_manip, m_record.seq);
                    m_skmer_iterator = m_seq_rator.begin();
                }
                else
                {
                    // No more sequence to read from the file
                    m_ptr = nullptr;
                    return;
                }
            }
            while (m_skmer_iterator.consumed());
        }

        friend class FileSkmerator;

    public:
        // Return kmer by value
        Skmer<kuint> operator*()
        {
            return m_seq_rator.current_skmer();
        }

        Iterator& operator++()
        {
            // File already consumed
            if (m_ptr == nullptr)
                return *this;

            // Go to next sequence
            if (m_skmer_iterator.consumed())
            {
                this->init_record();
                
                // reached the end of the file while looking for the next sequence
                if (m_ptr == nullptr)
                    return *this;
            }

            m_skmer_iterator.operator++();
            return *this;
        }


        bool operator==(const Iterator& it) const
        {
            return m_rator.m_filename == it.m_rator.m_filename and m_ptr == it.m_ptr;
        }

    };
    

    Iterator begin() { return Iterator(*this); }
    Iterator end() { return Iterator(*this, nullptr); }
};


}
#endif 