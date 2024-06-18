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

template <typename kuint>
class compare_kmer_skmer_pos;


template <typename kuint>
const std::vector<uint64_t> sort_kmers(const SkmerManipulator<kuint>& m_manip, const std::vector<Skmer<kuint> > m_skmer_list, uint64_t position){
    //storing a list of skmer_ids.
    std::vector<uint64_t> skmer_ids;
    std::iota (skmer_ids.begin(), skmer_ids.end(), 0); // Fill with 0, 1, ..., m_skmer_list.size() - 1
    
    std::vector<std::vector<uint64_t> > sorted_lists((2 * m_manip.k) - m_manip.m);
    // std::vector<std::vector<uint64_t> > sorted_lists((2 * 5) - 2);
    sorted_lists[position] = get_sorted_list(m_manip, m_skmer_list, position, skmer_ids);

    return sorted_lists[position];
}

template <typename kuint>
const std::vector<uint64_t> get_sorted_list(const SkmerManipulator<kuint>& m_manip, const std::vector<Skmer<kuint> > m_skmer_list
, const uint64_t position, const std::vector<uint64_t>& skmer_ids){
    // Accessing and comparing kmers in skmers (less than) is done by kmer_lt_kmer of skmermanipulator
    // 1st pass over the column: check which skmers are ok to be processed
    // Check if the first skmer has a kmer in this position
    std::vector<uint64_t> valid_skmer_ids;
    for(const uint64_t& skmer_id : skmer_ids){
        if (has_valid_skmer(m_manip, position, m_skmer_list[skmer_id])){
            valid_skmer_ids.push_back (skmer_id);
        }
    }
    std::cout << "VALID SKMER LIST:" << std::endl;
    for (auto i: valid_skmer_ids)
        std::cout << i << ' ';
    std::cout << std::endl;

    // 2nd pass over the column: return ordered list 
    // For every "column" i.e. possible kmer in the skmer size
    // For every skmer that has a kmer in that column
    //std::sort(valid_skmer_ids.begin(), valid_skmer_ids.end(), compare_kmer_skmer_pos<kuint, N>(position, m_manip, m_skmer_list));
    
    std::sort(valid_skmer_ids.begin(), valid_skmer_ids.end(),
               compare_kmer_skmer_pos<kuint>(position, m_manip, m_skmer_list));

    std::cout << "SORTED SKMER LIST:" << std::endl;
    for (auto i: valid_skmer_ids)
        std::cout << i << ' ';
    std::cout << std::endl;
    
    return valid_skmer_ids;
}

template <typename kuint>
class compare_kmer_skmer_pos {
    uint64_t position;
    const SkmerManipulator<kuint> & manipulator;
    const std::vector<Skmer<kuint> > & m_skmer_list;

public:
    // the comparison function takes as argument 2 integers, a position and the vector of skmers. 
    // It compares the two skmers in the selected position and returns which one is before the other.
    compare_kmer_skmer_pos(uint64_t p, const SkmerManipulator<kuint> & skmer_manipulator, const std::vector<Skmer<kuint> > & skmer_list) // 
    : position(p), manipulator(skmer_manipulator), m_skmer_list(skmer_list)  {}

    bool operator()(const uint64_t skmer_id_1,const uint64_t skmer_id_2) const {
        return manipulator.kmer_lt_kmer(m_skmer_list[skmer_id_1], position, m_skmer_list[skmer_id_2], position);
    }
};
}

#endif