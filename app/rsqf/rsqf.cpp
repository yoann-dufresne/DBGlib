#include <iostream>
#include <cstdint>
#include "cxxopts.hpp"

using namespace std;


int main(int argc, char const *argv[])
{
    // --- Init options ---
    cxxopts::Options options("rsqf", "Example usages of rsqf datastructure for kmers. During exec a minimal size RSQF is created and filled.");
    options.add_options()
        ("k", "kmer size [mandatory]", cxxopts::value<uint64_t>())
        ("f,fasta", "Fasta file to index", cxxopts::value<std::string>())
        ("v,verbose", "Verbose output", cxxopts::value<bool>()->default_value("false"))
        ("h,help", "Print usage")
    ;

    auto result = options.parse(argc, argv);

    // --- Mandatory options ---
    if (not result.count("k"))
    {
        cerr << "kmer size is absent " << endl;
        options.help();
        exit(1);
    }

    // --- Print help on request ---
    if (result.count("help"))
    {
        cout << options.help() << endl;
        exit(0);
    }

    // --- Program begin ---

    cout << "Enumeration of kmers of size " << result["k"].as<uint64_t>();
    cout << " to create the index." << endl;

    // Index creation
    

    // Query the index

    return 0;
}