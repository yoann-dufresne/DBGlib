#include <iostream>
#include <cstdlib>
#include <gtest/gtest.h>
#include <string>
#include <vector>

#include <io/Skmer.hpp>
#include <io/Skmerator.hpp>

using namespace std;



TEST(Skmerator, decreasing_minimizer)
{
    using kuint = uint16_t;
    using kpair = km::Skmer<kuint>::pair;
    
    const uint64_t k{5};
    const uint64_t m{2};

    km::SkmerManipulator<kuint> manip {k, m};
    std::string seq{"CCCCAAAAA"};
    km::SeqSkmerator<kuint> skmerator {manip, seq};
    km::SkmerPrettyPrinter<kuint> pp {k, m};
    

    //                         Prefix:         C   C   C   C             A   C   C   C             A   A   C   _  
    //                         Suffix:       A   _   _   _             A    A   _   _             A   A   A   _
    const kuint expected_values[][2] { {0, 0b0001110111011101U}, {0, 0b0000000111011101U}, {0, 0b0000000000011111U}
    };

    // FOR FASTA_3 WE EXPECT
    //const kuint expected_values_fa3[2] {0b}

    uint64_t nb_skmer {0};
    for ([[maybe_unused]]km::Skmer<kuint> skmer : skmerator)
    {
        // pp << skmer;
        // cout << pp << endl;

        //                            Less significant             Most significant
        const kpair expected_pair{expected_values[nb_skmer][1], expected_values[nb_skmer][0]};
        ASSERT_EQ(expected_pair, skmer.m_pair);
        
        nb_skmer += 1;
    }
    EXPECT_EQ(nb_skmer, 3);
}


TEST(Skmerator, increasing_minimizer)
{
    using kuint = uint16_t;
    using kpair = km::Skmer<kuint>::pair;
    
    const uint64_t k{5};
    const uint64_t m{2};

    km::SkmerManipulator<kuint> manip {k, m};
    std::string seq{"AACCCC"};
    km::SeqSkmerator<kuint> skmerator {manip, seq};
    km::SkmerPrettyPrinter<kuint> pp {k, m};
    

    //                         Prefix:         A   _   _   _             A   _   _   _   
    //                         Suffix:       A   C   C   C             C   C   C   C     
    const kuint expected_values[][2] { {0, 0b0000011101110111U}, {0, 0b0100011101110111U}
    };

    // FOR FASTA_3 WE EXPECT
    //const kuint expected_values_fa3[2] {0b}

    uint64_t nb_skmer {0};
    for ([[maybe_unused]]km::Skmer<kuint> skmer : skmerator)
    {
        pp << skmer;
        // cout << pp << endl;

        //                            Less significant             Most significant
        const kpair expected_pair{expected_values[nb_skmer][1], expected_values[nb_skmer][0]};
        ASSERT_EQ(expected_pair, skmer.m_pair);
        
        nb_skmer += 1;
    }

    EXPECT_EQ(nb_skmer, 2);
}


TEST(Skmerator, outofcontext_minimizer)
{
    using kuint = uint16_t;
    using kpair = km::Skmer<kuint>::pair;
    
    const uint64_t k{8};
    const uint64_t m{2};

    km::SkmerManipulator<kuint> manip {k, m};
    std::string seq{"AACAATAAGGGGGGG"};
    cout << seq << endl;
    km::SeqSkmerator<kuint> skmerator {manip, seq};
    km::SkmerPrettyPrinter<kuint> pp {k, m};
    

    //                         Prefix:      A   _   _        _   _   _   _          A   C   A       _   _   _   _   
    //                         Suffix:    A   C   A        A   T   A   A          A   T   A       A   G   G   G
    const kuint expected_values[][2] { {0b000001110011U, 0b0011101100110011U}, {0b000010010000U, 0b0011111111111111U}
        // Prefix:     A   T   A        _   _   _   _          C   C   C       C   _   _   _
        // Suffix:   A   G   G        G   G   G   G          C   C   C       T   _   _   _
        ,         {0b000011101100U, 0b1111111111111111U}, {0b010101010101, 0b1001111111111111U}
    };

    // FOR FASTA_3 WE EXPECT
    //const kuint expected_values_fa3[2] {0b}

    uint64_t nb_skmer {0};
    for ([[maybe_unused]]km::Skmer<kuint> skmer : skmerator)
    {
        pp << skmer;
        cout << pp << endl;

        //                            Less significant             Most significant
        const kpair expected_pair{expected_values[nb_skmer][1], expected_values[nb_skmer][0]};
        ASSERT_EQ(expected_pair, skmer.m_pair);
        
        nb_skmer += 1;
    }

    ASSERT_EQ(nb_skmer, 4);
}


TEST(Skmerator, file_vs_seq)
{
    using kuint = uint16_t;
    
    const uint64_t k{5};
    const uint64_t m{2};
    km::SkmerPrettyPrinter<kuint> pp {k, m};

    // --- Sequence ---
    std::string seq{"CCCCAAAAA"};
    km::SkmerManipulator<kuint> seq_manip {k, m};
    km::SeqSkmerator<kuint> seq_skmerator {seq_manip, seq};

    // Enumerates the superkmers from the sequence
    std::vector<km::Skmer<kuint> > seq_skmers {};
    for (km::Skmer<kuint> skmer : seq_skmerator)
        seq_skmers.push_back(skmer);

    // --- File ---
    std::string filename{"../tests/data/CCCCAAAAA.fa"};
    km::SkmerManipulator<kuint> file_manip {k, m};
    km::FileSkmerator<kuint> file_skmerator {file_manip, filename};


    // Enumerates the superkmers from the file
    std::vector<km::Skmer<kuint> > file_skmers {};
    for (km::Skmer<kuint> skmer : file_skmerator)
        file_skmers.push_back(skmer);
    
    // Comparison of size
    if (seq_skmers.size() != file_skmers.size())
    {
        std::cerr << "from sequence" << std::endl;
        for (const auto& skmer : seq_skmers)
        {
            pp << skmer;
            std::cerr << pp << " ";
        } std::cerr << std::endl;

        std::cerr << "from file" << std::endl;
        for (const auto& skmer : file_skmers)
        {
            pp << skmer;
            std::cerr << pp << " ";
        } std::cerr << std::endl;
    }
    ASSERT_EQ(seq_skmers.size(), file_skmers.size());

    // Compare skmers one by one
    for (uint64_t i{0} ; i<file_skmers.size() ; i++)
        ASSERT_TRUE(seq_manip.skmer_equals(seq_skmers[i], file_skmers[i]));
}