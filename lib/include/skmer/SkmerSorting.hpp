#include <array>
#include <vector>
#include <cstdint>
#include <algorithm>

#include <io/Skmer.hpp>
#include <io/Skmerator.hpp>

#ifndef SKMERSORTING_H
#define SKMERSORTING_H


namespace km
{
template <typename kuint>
class SkmerSorting
{
protected:
    const SkmerManipulator<kuint>& m_manip;
    const std::vector<Skmer<kuint>> m_skmer_list;

public:
    SkmerSorting(const std::vector<Skmer<kuint>> enumerated_superkmers, SkmerManipulator<kuint>& skmer_manipulator)
        : m_skmer_list(enumerated_superkmers), m_manip(skmer_manipulator)
    {};

    std::pair<std::vector<uint64_t>,uint64_t> sort_skmer(){
        std::vector<uint64_t> skmer_ids(m_skmer_list.size());
        std::iota (std::begin(skmer_ids), std::end(skmer_ids), 0); // Fill with 0, 1, ..., m_skmer_list.size() - 1
    }

    const std::vector<Skmer<kuint>> sorted_skmers get_sorted_list(){
        // storing a list of skmer_ids.
        std::vector<uint64_t> skmer_ids(m_skmer_list.size());
        std::iota (std::begin(skmer_ids), std::end(skmer_ids), 0); // Fill with 0, 1, ..., m_skmer_list.size() - 1

        // 1) Accessing and comparing kmers in skmers (less than) is done by kmer_lt_kmer of skmermanioulator
        // 2) the comparison function takes as argument 2 integers, and a position. It compares the two skmers in the 
        

        // For every "column" i.e. possible kmer in the skmer size
        // For every skmer that has a kmer in that column
        for (uint64_t pos, pos < 2 * m_manip.k - m_manip.m; pos++){
            std::sort(skmer_ids, my_vector_last, compare_kmer_skmer_pos);
        }

        std::sort(numbers.begin(), numbers.end(), [&comp](int a, int b) {
            return comp.compare(a, b);
        });


    }
}
}

class compare_kmer_skmer_pos {
    const SkmerManipulator<kuint>& manipulator;
    const uint64_t position;
public:
    compare_kmer_skmer_pos(uint64_t p, SkmerManipulator<kuint>& skmer_manipulator) 
    : position(p), manipulator(skmer_manipulator)  {}

    bool operator()(uint64_t skmer_id_1, uint64_t skmer_id_2) {
        // I need to check prefix & suffix of the skmer
        Skmer<kuint> sk1(m_skmer_list[skmer_id_1]);
        Skmer<kuint> sk2(m_skmer_list[skmer_id_2]);

        if (prefix > manipulator.)
        // Check if the first skmer has a kmer in this position
        if (position >= manipulator.prefix - sk1.prefix){

        }
        
        // Check if the second skmer has a kmer in this position

        // else compare the 2 kmers
        return manipulator.kmer_lt_kmer(sk1, position, sk2, position);
    }
};
