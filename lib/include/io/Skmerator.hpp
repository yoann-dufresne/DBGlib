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
    Skmer<kuint> m_prev_min_skmer;

public:
    FileSkmerator(const std::string& filename, SkmerManipulator<kuint>& manipulator)
        : m_filename(filename), m_manip(manipulator)
    {};

    struct Iterator
    {
    protected:
        // Construct an iterator without control on the file stream
        Iterator(FileSkmerator& skmerator, std::unique_ptr<klibpp::SeqStreamIn> stream_ptr)
            : m_rator(skmerator), m_ptr(std::move(stream_ptr)), m_manip(skmerator.m_manip)
            , m_remaining_nucleotides(0), m_seq_idx(0)
            , m_skmer_buffer_array(new Skmer<kuint>[skmerator.m_manip.k - skmerator.m_manip.m + 1])
            , m_ptr_start(0), m_ptr_stop(skmerator.m_manip.k - skmerator.m_manip.m)
            , m_min_pos(0)
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
        
        ~Iterator()
        {
            delete[] m_skmer_buffer_array;
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
                    for (m_seq_idx = 0 ; m_seq_idx<m_manip.k-1 ; m_seq_idx++)
                    {
                        // Nucl encoding. TODO: Move encoding to dedicated classes
                        const kuint nucl {(m_record.seq[m_seq_idx] >> 1) & 0b11U};
                        m_manip.add_nucleotide(nucl);
                    }

                    m_curr_min_skmer = m_manip.get_max();
                    // while the skmer length is < than the one of the full max superkmer
                    for(uint64_t i {m_manip.k}; i <= (2*m_manip.k - m_manip.m); i++)
                    {
                        // add nucleotide to the superkmer
                        const kuint nucl {(m_record.seq[m_seq_idx] >> 1) & 0b11U};
                        const Skmer<kuint> candidate {m_manip.add_nucleotide(nucl)};

                        // fill the buffer with the new skmer candidate
                        m_skmer_buffer_array[i - m_manip.k] = candidate;

                        // remember the minimal candidate skmer in the array
                        if (candidate <= m_skmer_buffer_array[m_min_pos]){
                            m_min_pos = i - m_manip.k;
                        }

                    }
                    m_remaining_nucleotides = m_record.seq.length() - 2*m_manip.k + m_manip.m + 1;
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
            return m_rator.m_prev_min_skmer;
        }

        Iterator& operator++()
        {
            // File already consumed
            if (m_ptr == nullptr)
                return *this;

            // Go to next sequence
            if (m_remaining_nucleotides == 0)
            {
                this->init_record();
                // reached the end of the file while looking for the next sequence
                if (m_ptr == nullptr)
                    return *this;
            }

            while (m_remaining_nucleotides > 0)
            {
                // increment the position in the buffer
                m_ptr_stop = (m_ptr_stop + 1) % ( m_manip.k - m_manip.m + 1);
                
                // out of context case
                if (m_ptr_start == m_ptr_stop)
                {
                    // store the previous skmer
                    m_rator.m_prev_min_skmer = m_skmer_buffer_array[m_min_pos];

                    //move the beginning to the new position
                    m_ptr_start = m_min_pos + m_manip.k - m_manip.m;

                    //compute the new minimal skmer in the remaining skmers
                    

                }


                m_seq_idx += 1;
                m_remaining_nucleotides -= 1;
                // Get the next kmer
                const kuint nucl {(m_record.seq[m_seq_idx] >> 1) & 0b11U};
                
                // add nucleotide to the current candidate superkmer
                Skmer<kuint> candidate {m_manip.add_nucleotide(nucl)};

                // If there is a new minimal superkmer
                if (candidate <= m_curr_min_skmer){ // USE <= !!

                    // registering the current minimal skmer to yield it
                    m_rator.m_prev_min_skmer = m_curr_min_skmer;

                    // update the boundaries of the superkmer to be yielded


                    // save the new current minimal skmer
                    m_curr_min_skmer = candidate;

                    break;
                }

            }

            // We reached the end of the sequence
            m_rator.m_prev_min_skmer = m_curr_min_skmer;

            return *this;
        }

        bool operator==(const Iterator& it) const
        {
            return m_rator.m_filename == it.m_rator.m_filename and m_ptr == it.m_ptr;
        }


    private:
        FileSkmerator<kuint>& m_rator;
        std::unique_ptr<klibpp::SeqStreamIn> m_ptr;
        SkmerManipulator<kuint>& m_manip;
        Skmer<kuint> m_curr_min_skmer;
        klibpp::KSeq m_record;
        uint64_t m_remaining_nucleotides;
        uint64_t m_seq_idx;
        Skmer<kuint>* m_skmer_buffer_array;
        uint64_t m_ptr_start;
        uint64_t m_ptr_stop;
        uint64_t m_min_pos;
    };
    

    Iterator begin() { return Iterator(*this); }
    Iterator end() { return Iterator(*this, nullptr); }
};


}
#endif 