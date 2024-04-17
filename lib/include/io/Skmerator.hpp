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
class FileSkmerator
{
protected:
    const std::string& m_filename;
    SkmerManipulator<kuint>& m_manip;
    Skmer<kuint> m_yielded_skmer;

public:
    FileSkmerator(const std::string& filename, SkmerManipulator<kuint>& manipulator)
        : m_filename(filename), m_manip(manipulator)
    {};

    struct Iterator
    {
    public:
        ~Iterator()
        {
            delete[] m_skmer_buffer_array;
        }

    protected:
        // Construct an iterator without control on the file stream
        Iterator(FileSkmerator& skmerator, std::unique_ptr<klibpp::SeqStreamIn> stream_ptr)
            : m_rator(skmerator), m_ptr(std::move(stream_ptr)), m_remaining_nucleotides(0)
            , m_manip(skmerator.m_manip), m_current_minimizer(~static_cast<kuint>(0))
            , m_buffer_size(2 * skmerator.m_manip.k - skmerator.m_manip.m)
            , m_skmer_buffer_array(new Skmer<kuint>[m_buffer_size])
            , m_ptr_current(skmerator.m_manip.k - skmerator.m_manip.m)
            , m_ptr_min(0) // minimizer_position
        {
            if (m_ptr == nullptr)
                return;

            this->init_record();
            //cout << m_manip << endl;
            this->operator++();

            // const kuint nucl {(m_record.seq[m_seq_idx] >> 1) & 0b11U};
            // m_curr_min_skmer = m_manip.add_nucleotide(nucl);

            // m_remaining_nucleotides -= 1;
            // m_seq_idx += 1;
        }

        // Construct a new file stream from the filename
        Iterator(FileSkmerator& skmerator)
            : Iterator ( skmerator, std::make_unique<klibpp::SeqStreamIn>(skmerator.m_filename.c_str()) )
        {}

        void init_record()
        {
            cout << "init_record" << endl;
            do
            {
                if ((*m_ptr) >> m_record)
                {
                    // If sequence is too short, skip it
                    if (m_record.seq.length() < m_rator.m_manip.k) {
                        continue;
                    }

                    // Init the first kmer                    m_manip.init_skmer();
                    for (uint64_t idx = 0 ; idx<m_manip.k-1 ; idx++)
                    {
                        // Nucl encoding. TODO: Move encoding to dedicated classes
                        const kuint nucl {static_cast<kuint>((m_record.seq[idx] >> 1) & 0b11U)};
                        m_manip.add_nucleotide(nucl);
                    }

                    m_current_minimizer = ~static_cast<kuint>(0);

                    m_ptr_current = m_rator.m_manip.k - 2;
                    m_ptr_min = m_rator.m_manip.k - 2;
                    m_remaining_nucleotides = m_record.seq.length() - m_manip.k + 1;
                }
                else
                {
                    // No more sequence to read from the file
                    m_ptr = nullptr;
                    return;
                }
            }
            while (m_record.seq.length() < m_manip.k);
            // cout << m_curr_min_skmer << endl;
            cout << "/init_record" << endl;

        }

        friend class FileSkmerator;

    public:
        // Return kmer by value
        Skmer<kuint> operator*() const
        {
            cout << "operator*" << endl;
            return m_rator.m_yielded_skmer;
        }

        Iterator& operator++()
        {
            cout << "operator++" << endl;
            // File already consumed
            if (m_ptr == nullptr)
                return *this;

            const uint64_t k {m_rator.m_manip.k};
            const uint64_t m {m_rator.m_manip.m};

            // Go to next sequence
            if (m_remaining_nucleotides + k - m == 0)
            {
                this->init_record();
                // reached the end of the file while looking for the next sequence
                if (m_ptr == nullptr)
                    return *this;
            }


            while (m_remaining_nucleotides + k - m > 0)
            {
                // -- Save the skmer to eventually yield
                m_rator.m_yielded_skmer = m_skmer_buffer_array[(m_ptr_current + 1) % m_buffer_size];

                // -- On out of context minimizer: recompute the previous minimizer

                // -- Compute the new candidate skmer
                m_remaining_nucleotides -= 1;
                m_ptr_current += 1;

                // Get the next skmer 
                // If we reach the end of the sequence we add fake nucleotides (0b11) to complete the last skmer  
                const kuint nucl {
                    m_remaining_nucleotides >= 0 ?(static_cast<kuint>((m_record.seq[m_ptr_current] >> 1) & 0b11U)) : static_cast<kuint>(0b11U)};
                cout << "m: " << m_ptr_min << " ; c: " << m_ptr_current << endl;
                
                // add nucleotide to the current candidate superkmer
                m_skmer_buffer_array[ m_ptr_current % m_buffer_size ] = m_manip.add_nucleotide(nucl);
                Skmer<kuint>& candidate {m_skmer_buffer_array[ m_ptr_current % m_buffer_size ]};
                
                // Get the minimizer
                const kuint candidate_minimizer {m_manip.minimizer(candidate)};
                // cout << "candidate_minimizer: " << candidate_minimizer << " m_current_minimizer: " << m_current_minimizer << endl;

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
                    }
                    
                    // 4 - save the new current minimal skmer and minimizer
                    m_ptr_min = m_ptr_current;
                    m_current_minimizer = candidate_minimizer;
                }

                // The candidate minimizer is the same than the previous minimizer
                else if (candidate_minimizer == m_current_minimizer)
                {
                    const uint64_t pos_diff {m_ptr_current - m_ptr_min};
                    Skmer<kuint>& current_skmer {m_skmer_buffer_array[m_ptr_current % m_buffer_size]};
                    Skmer<kuint>& prev_skmer {m_skmer_buffer_array[m_ptr_min % m_buffer_size]};

                    // 0 - Due to the way that we encode skmers, the no suffix kmer in the current skmer will always be with the previous skmer.
                    uint64_t first_kmer_in_new_skmer {1};

                    // 1 - Scan all the kmer shared between the 2 skmers to decide where is the limit between the 2 skmers
                    for (uint64_t curr_skmer_km_pos {1} ; curr_skmer_km_pos < k - m - pos_diff ; curr_skmer_km_pos++)
                    {
                        const uint64_t prev_skmer_km_pos {curr_skmer_km_pos + pos_diff};

                        // 2 - The shared kmer is part of the new skmer
                        if (m_manip.kmer_lt_kmer(current_skmer, curr_skmer_km_pos, prev_skmer, prev_skmer_km_pos))
                        {
                            first_kmer_in_new_skmer = curr_skmer_km_pos;
                            break;
                        }
                    }

                    // 3 - Update suffix of the previous skmer and prefix of the new skmer
                    prev_skmer.m_suff_size = pos_diff + first_kmer_in_new_skmer;
                    current_skmer.m_pref_size = k - m - first_kmer_in_new_skmer;

                    m_ptr_min = m_ptr_current;
                }

                // -- Yield if needed
                cout << "registered skmer " << m_rator.m_yielded_skmer << endl;
                if (m_rator.m_yielded_skmer.m_pref_size + m_rator.m_yielded_skmer.m_suff_size >= k - m)
                {
                    m_manip.mask_absent_nucleotides(m_rator.m_yielded_skmer);
                    return *this;
                }

            } // End of the while over the sequence

            cout << endl;
            for (uint64_t idx{0} ; idx<m_buffer_size ; idx++)
            {
                cout << ((m_ptr_current + 1 + idx) % m_buffer_size) << "\t" << m_skmer_buffer_array[(m_ptr_current + 1 + idx) % m_buffer_size] << endl;
            }
            cout << endl;
            exit(1);

            // We reached the end of the sequence
            m_rator.m_yielded_skmer = m_skmer_buffer_array[m_ptr_min % m_buffer_size];
            m_rator.m_yielded_skmer.m_suff_size = m_ptr_current - m_ptr_min - 1;
            
            cout << "Last one." << endl;
            return *this;
        }


        bool operator==(const Iterator& it) const
        {
            return m_rator.m_filename == it.m_rator.m_filename and m_ptr == it.m_ptr;
        }


    private:
        // Sequence file related attributes
        FileSkmerator<kuint>& m_rator;
        std::unique_ptr<klibpp::SeqStreamIn> m_ptr;
        klibpp::KSeq m_record;
        int64_t m_remaining_nucleotides;

        // Manipulator to compute interleaved along the sequence
        SkmerManipulator<kuint>& m_manip;
        kuint m_current_minimizer;
        // Skmer<kuint> m_curr_min_skmer;

        // Buffered unmasked skmers
        const uint64_t m_buffer_size;
        Skmer<kuint>* m_skmer_buffer_array;
        uint64_t m_ptr_current;
        uint64_t m_ptr_min;
    };
    

    Iterator begin() { return Iterator(*this); }
    Iterator end() { return Iterator(*this, nullptr); }
};


}
#endif 