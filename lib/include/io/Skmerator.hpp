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
            , m_manip(skmerator.m_manip), m_prefixed_skmer({0,0}), m_current_minimizer(~static_cast<kuint>(0))
            , m_buffer_size(2 * skmerator.m_manip.k - skmerator.m_manip.m)
            , m_skmer_buffer_array(new Skmer<kuint>[m_buffer_size])
            , m_ptr_begin(0), m_ptr_end(skmerator.m_manip.k - skmerator.m_manip.m)
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

                    m_ptr_begin = m_rator.m_manip.k - 1;
                    m_ptr_end = m_rator.m_manip.k - 2;
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
                // cout << "remaining_nucleotides: " << m_remaining_nucleotides << " ; k: " << k << " ; m: " << m << endl;
                this->init_record();
                // reached the end of the file while looking for the next sequence
                if (m_ptr == nullptr)
                    return *this;
            }


            while (m_remaining_nucleotides + k - m > 0)
            {                
                cout << "seq position: " << (m_ptr_end+1) << " ; rem_nuc " << m_remaining_nucleotides << endl;

                // out of context case (the suffix of the skmer is not large enought to hold the minimizer)
                // TODO : Reread everything here !
                if (m_ptr_end - m_ptr_min >= k - m)
                {
                    // store the previous skmer
                    m_prefixed_skmer = m_skmer_buffer_array[ m_ptr_min % m_buffer_size ];

                    //move the beginning to the new position
                    m_ptr_begin = m_ptr_min + 1;

                    //compute the new minimal skmer in the remaining skmers
                    // TODO: Redo it with minimizers intead of skmer comparison
                    m_current_minimizer = ~static_cast<kuint>(0);
                    for(uint64_t abs_pos {m_ptr_begin}; abs_pos <= m_ptr_end; abs_pos++)
                    {
                        const kuint buffer_mini {m_manip.minimizer(m_skmer_buffer_array[abs_pos % m_buffer_size])};
                        if (buffer_mini <= m_current_minimizer)
                        {
                            m_current_minimizer = buffer_mini;
                            m_ptr_min = abs_pos;
                        }
                    }
                    // cout << "b: " << m_ptr_begin << " ; m: " << m_ptr_min << " ; e: " << m_ptr_end << endl;
                    cout << "Yielding OCC end-curr >= k - m" << endl;
                    return *this;
                }

                m_remaining_nucleotides -= 1;
                m_ptr_end += 1;

                // Get the next kmer 
                // If we reach the end of the sequence we add fake nucleotides (0b11) to complete the last skmer  
                const kuint nucl {
                    m_remaining_nucleotides >= 0 ?(static_cast<kuint>((m_record.seq[m_ptr_end] >> 1) & 0b11U)) : static_cast<kuint>(0b11U)};
                cout << "b: " << m_ptr_begin << " ; m: " << m_ptr_min << " ; e: " << m_ptr_end << endl;
                
                // add nucleotide to the current candidate superkmer
                Skmer<kuint> candidate {m_manip.add_nucleotide(nucl)};
                m_skmer_buffer_array[ m_ptr_end % m_buffer_size ] = candidate;
                

                const kuint candidate_minimizer {m_manip.minimizer(candidate)};
                cout << "candidate_minimizer: " << candidate_minimizer << " m_current_minimizer: " << m_current_minimizer << endl;

                // If there is a new minimal superkmer
                if (candidate_minimizer < m_current_minimizer){

                    // out of context 
                    if ( m_ptr_end - m_ptr_begin > k - m)
                    {
                        // 1 - Yield the prefied registered skmer
                        m_prefixed_skmer.m_suff_size = m_ptr_end - m_ptr_min - 1;
                        m_rator.m_yielded_skmer = m_prefixed_skmer;

                        // 2 - Prefix the new skmer
                        candidate.m_pref_size = k - m;
                        m_prefixed_skmer = candidate;

                        // update the boundaries of the superkmer to be yielded
                        m_ptr_begin = m_ptr_min + 1;
                        
                        // save the new current minimal skmer
                        m_ptr_min = m_ptr_end;
                        
                        // cout << "b: " << m_ptr_begin << " ; m: " << m_ptr_min << " ; e: " << m_ptr_end << endl;
                        // cout << "Yielding OCC end-beg > k - m" << endl;

                        // 3 - return the yielded skmer
                        return *this;
                    }

                    // save the new current minimal skmer
                    m_current_minimizer = candidate_minimizer;
                    m_ptr_min = m_ptr_end;
                }

                // The candidate is the same than the previous minimizer
                else if (candidate_minimizer == m_current_minimizer)
                {
                    cerr << "equal minimizer not implemented yet" << endl;
                    exit(1);
                }
            }

            // We reached the end of the sequence
            m_prefixed_skmer.m_suff_size = m_ptr_end - m_ptr_min - 1;
            m_rator.m_yielded_skmer = m_prefixed_skmer;

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
        // Beginning of a skmer not yielded yet
        Skmer<kuint> m_prefixed_skmer;
        kuint m_current_minimizer;
        // Skmer<kuint> m_curr_min_skmer;

        // Buffered unmasked skmers
        const uint64_t m_buffer_size;
        Skmer<kuint>* m_skmer_buffer_array;
        uint64_t m_ptr_begin;
        uint64_t m_ptr_end;
        uint64_t m_ptr_min;
    };
    

    Iterator begin() { return Iterator(*this); }
    Iterator end() { return Iterator(*this, nullptr); }
};


}
#endif 