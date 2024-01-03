#include <iostream>
#include <string>
#include <kseq++/seqio.hpp>


#ifndef KMER_IN_H
#define KMER_IN_H


//template <uint64_t k>
class KmerStreamIn
{
protected:
    klibpp::SeqStreamIn iss;
    klibpp::KSeq record {};
public:
    KmerStreamIn(const std::string& filename) : iss(filename.c_str())
    {
        while (iss >> record)
        {
            std::cout << record.name << std::endl;
            std::cout << record.seq << std::endl;
        }
    };
};


#endif
