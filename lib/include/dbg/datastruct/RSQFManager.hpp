#include <cstdlib>
#include <dbg/datastruct/RSQF.hpp>


#ifndef RSQF_MNG_H
#define RSQF_MNG_H

template<uint64_t uint_size>
class QF
{
public:
    uint64_t current_q;

    QF() : current_q (6)
    {
        m_qf = new QuotientFilter<6, uint_size-6>();
    };

    void insert(const uint64_t* begin, const uint64_t* end) {
        uint64_t* next = begin;
        do {
            next = std::visit([&](auto&& arg) -> uint64_t* {
                return arg->insert(next, end);
            }, m_qf);

            if (next != end) {
                resize();
            }
        } while (next != end);
    };


    void resize()
    {
        auto that = this;
        std::visit([&](auto&& arg) -> void {
            // Create a new Quotient Filter with the double size
            std::unique_ptr<QuotientFilter> newFilter = std::make_unique<QuotientFilter>(*arg);
            // Deallocate the previous filter and make the new filter the new attribute
            arg.reset(std::move(newFilter));
        }, m_qf);

        current_q += 1;
    };


protected:
    std::variant<
        std::unique_ptr<QuotientFilter<6, uint_size - 6>>,
        std::unique_ptr<QuotientFilter<7, uint_size - 7>>,
        std::unique_ptr<QuotientFilter<8, uint_size - 8>>,
        std::unique_ptr<QuotientFilter<9, uint_size - 9>>,
        std::unique_ptr<QuotientFilter<10, uint_size - 10>>,
        std::unique_ptr<QuotientFilter<11, uint_size - 11>>,
        std::unique_ptr<QuotientFilter<12, uint_size - 12>>,
        std::unique_ptr<QuotientFilter<13, uint_size - 13>>,
        std::unique_ptr<QuotientFilter<14, uint_size - 14>>,
        std::unique_ptr<QuotientFilter<15, uint_size - 15>>,
        std::unique_ptr<QuotientFilter<16, uint_size - 16>>,
        std::unique_ptr<QuotientFilter<17, uint_size - 17>>,
        std::unique_ptr<QuotientFilter<18, uint_size - 18>>,
        std::unique_ptr<QuotientFilter<19, uint_size - 19>>,
        std::unique_ptr<QuotientFilter<20, uint_size - 20>>,
        std::unique_ptr<QuotientFilter<21, uint_size - 21>>,
        std::unique_ptr<QuotientFilter<22, uint_size - 22>>,
        std::unique_ptr<QuotientFilter<23, uint_size - 23>>,
        std::unique_ptr<QuotientFilter<24, uint_size - 24>>,
        std::unique_ptr<QuotientFilter<25, uint_size - 25>>,
        std::unique_ptr<QuotientFilter<26, uint_size - 26>>,
        std::unique_ptr<QuotientFilter<27, uint_size - 27>>,
        std::unique_ptr<QuotientFilter<28, uint_size - 28>>,
        std::unique_ptr<QuotientFilter<29, uint_size - 29>>,
        std::unique_ptr<QuotientFilter<30, uint_size - 30>>,
        std::unique_ptr<QuotientFilter<31, uint_size - 31>>,
        std::unique_ptr<QuotientFilter<32, uint_size - 32>>,
        std::unique_ptr<QuotientFilter<33, uint_size - 33>>,
        std::unique_ptr<QuotientFilter<34, uint_size - 34>>,
        std::unique_ptr<QuotientFilter<35, uint_size - 35>>,
        std::unique_ptr<QuotientFilter<36, uint_size - 36>>,
        std::unique_ptr<QuotientFilter<37, uint_size - 37>>,
        std::unique_ptr<QuotientFilter<38, uint_size - 38>>,
        std::unique_ptr<QuotientFilter<39, uint_size - 39>>,
        std::unique_ptr<QuotientFilter<40, uint_size - 40>>,
        std::unique_ptr<QuotientFilter<41, uint_size - 41>>,
        std::unique_ptr<QuotientFilter<42, uint_size - 42>>,
        std::unique_ptr<QuotientFilter<43, uint_size - 43>>,
        std::unique_ptr<QuotientFilter<44, uint_size - 44>>,
        std::unique_ptr<QuotientFilter<45, uint_size - 45>>,
        std::unique_ptr<QuotientFilter<46, uint_size - 46>>,
        std::unique_ptr<QuotientFilter<47, uint_size - 47>>,
        std::unique_ptr<QuotientFilter<48, uint_size - 48>>,
        std::unique_ptr<QuotientFilter<49, uint_size - 49>>,
        std::unique_ptr<QuotientFilter<50, uint_size - 50>>,
        std::unique_ptr<QuotientFilter<51, uint_size - 51>>,
        std::unique_ptr<QuotientFilter<52, uint_size - 52>>,
        std::unique_ptr<QuotientFilter<53, uint_size - 53>>,
        std::unique_ptr<QuotientFilter<54, uint_size - 54>>,
        std::unique_ptr<QuotientFilter<55, uint_size - 55>>,
        std::unique_ptr<QuotientFilter<56, uint_size - 56>>,
        std::unique_ptr<QuotientFilter<57, uint_size - 57>>,
        std::unique_ptr<QuotientFilter<58, uint_size - 58>>,
        std::unique_ptr<QuotientFilter<59, uint_size - 59>>,
        std::unique_ptr<QuotientFilter<60, uint_size - 60>>,
        std::unique_ptr<QuotientFilter<61, uint_size - 61>>,
        std::unique_ptr<QuotientFilter<62, uint_size - 62>>,
        std::unique_ptr<QuotientFilter<63, uint_size - 63>>,
        std::unique_ptr<QuotientFilter<64, uint_size - 64>>,
        std::unique_ptr<QuotientFilter<65, uint_size - 65>>,
        std::unique_ptr<QuotientFilter<66, uint_size - 66>>,
        std::unique_ptr<QuotientFilter<67, uint_size - 67>>,
        std::unique_ptr<QuotientFilter<68, uint_size - 68>>,
        std::unique_ptr<QuotientFilter<69, uint_size - 69>>
    > m_qf;
};

#endif