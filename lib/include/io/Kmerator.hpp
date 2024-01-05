#include <iostream>
#include <string>
#include <memory>
#include <kseq++/seqio.hpp>

#include <io/Kmer.hpp>


#ifndef KMERATOR_H
#define KMERATOR_H

using namespace std;


template <uint64_t k, typename kuint>
class FileKmerator
{
protected:
    const std::string& m_filename;
    // klibpp::SeqStreamIn& m_stream;
    // klibpp::KSeq record {};
    // uint64_t seq_idx;
public:
    FileKmerator(const std::string& filename) : m_filename(filename)
    {
        static_assert((k+3) / 4 < sizeof(kuint));
    };

    struct Iterator
    {
    protected:
        // Construct an iterator without control on the file stream
        Iterator(const std::string& filename, std::unique_ptr<klibpp::SeqStreamIn> stream_ptr)
            : m_filename(filename) , m_ptr(std::move(stream_ptr)), m_current_kmer(42)
        {}
        // Construct a new file stream from the filename
        Iterator(const std::string& filename)
            : m_filename(filename)
            , m_ptr( std::make_unique<klibpp::SeqStreamIn>(filename.c_str()) )
            , m_current_kmer(42)
        {
            klibpp::KSeq record;

            // cout << "eof " << m_ptr->eof() << endl;
            // cout << "fail " << m_ptr->fail() << endl;

            while ((*m_ptr) >> record)
            {
                std::cout << record.name << std::endl;
                std::cout << record.seq << std::endl;
            }
        }

        friend class FileKmerator;

    public:
        Kmer<kuint>& operator*()
        {
            return m_current_kmer;
        }

        Iterator& operator++()
        {
            // File already consumed
            if (m_ptr == nullptr)
                return *this;

            // Go to next sequence
            klibpp::KSeq record{};
            if ((*m_ptr) >> record)
            {
                std::cout << record.name << std::endl;
                std::cout << record.seq << std::endl;
            }
            else
            {
                // Invalidate the stream after reaching eof
                m_ptr = nullptr;
            }

            return *this;
        }

        bool operator==(const Iterator& it) const
        {
            return m_filename == it.m_filename and m_ptr == it.m_ptr;
        }


    private:
        const std::string& m_filename;
        std::unique_ptr<klibpp::SeqStreamIn> m_ptr;
        Kmer<kuint> m_current_kmer;
    };
    

    Iterator begin() { return Iterator(m_filename); }
    Iterator end() { return Iterator(m_filename, nullptr); }
};


#endif
