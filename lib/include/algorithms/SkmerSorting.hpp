#include <array>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <numeric>

#include <io/Skmer.hpp>
#include <io/Skmerator.hpp>

#ifndef SKMERSORTING_H
#define SKMERSORTING_H


namespace km
{
    namespace sorting
    {

    template <class It, typename kuint>
    class compare_kmer_skmer_pos;


    /** Sorts skmer ids based on the kmers they contain at a given positon.
     * @param start start_position in the skmer generator
     * @param end end_positon in the skmer generator
     * @param kmer_pos position of the kmer in the skmer (column position)
     * @param m_manip skmer manipulator
     * @return a vector of sorted superkmer ids (if no kmer, no skmer id) 
     **/
    template<class It, typename kuint>
    std::vector<uint64_t> sort_column(It start, It end, uint64_t kmer_pos, SkmerManipulator<kuint>& m_manip)
    {   
        // Accessing and comparing kmers in skmers (less than) is done by kmer_lt_kmer of skmermanipulator
        // 1st pass over the column: check which skmers are ok to be processed
        // Check if the first skmer has a kmer in this position
        std::vector<uint64_t> valid_skmer;
        uint64_t sk_id = 0;
        //Iterating over the range [start, end)
        for(It skmer = start; skmer != end; ++skmer)
        {
            if (m_manip.has_valid_kmer(*skmer, kmer_pos))
                valid_skmer.push_back(sk_id);
            sk_id++; 
        }

        // 2nd pass over the column: return ordered list 
        // For every "column" i.e. possible kmer in the skmer size
        // For every skmer that has a kmer in that column
        std::sort(valid_skmer.begin(), valid_skmer.end(),
                compare_kmer_skmer_pos<It, kuint>(kmer_pos, m_manip, start, end));

        std::cout << "SORTED SKMER LIST:" << std::endl;
        for (auto i: valid_skmer)
            std::cout << i << ' ';
        std::cout << std::endl;
        
        return valid_skmer;
    }

    /** Returns candidate overlaps between two columns of sorted skmer ids
     * @param left_column first column
     * @param right_column second column (contigous)
     * @return a vector of pairs of candidate overlaps between the two columns
     **/
    template<class It, typename kuint>
    std::vector<std::pair<uint64_t, uint64_t> >  get_candidate_overlaps(std::vector<uint64_t> left_column, std::vector<uint64_t> right_column)
    {
        
    }


    template <class It, typename kuint>
    class compare_kmer_skmer_pos {
        uint64_t position;
        SkmerManipulator<kuint> & manipulator;
        const It start;
        const It end;


    public:
        // the comparison function takes as argument 2 integers, a position and the vector of skmers. 
        // It compares the two skmers in the selected position and returns which one is before the other.
        compare_kmer_skmer_pos(uint64_t p, SkmerManipulator<kuint> & skmer_manipulator, const It start_skmer_en, const It end_skmer_en) // 
        : position(p), manipulator(skmer_manipulator), start(start_skmer_en), end(end_skmer_en) {}

        bool operator()(const uint64_t skmer_id_1,const uint64_t skmer_id_2) const {
            assert((start+skmer_id_1) < end);
            assert((start+skmer_id_2) < end);
            return manipulator.kmer_lt_kmer(*(start+skmer_id_1), position, *(start+skmer_id_2), position);
        }
    };
    }
}

#endif