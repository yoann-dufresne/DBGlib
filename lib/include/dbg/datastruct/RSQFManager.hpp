#include <cstdlib>
#include <dbg/datastruct/RSQF.h>


#ifndef RSQF_MNG_H
#define RSQF_MNG_H

template<uint64_t uint_size>
class QF
{
public:
    uint64_t current_q;

    QF() : current_q (uint_size - 6)
    {
        
    };

    /** Insert a list of uints in the quotient filter.
     * Resize the quotient filter if needed
     * @param begin First element of the list
     * @param end Last element of the list
     **/
    void insert(const uint64_t * begin, const uint64_t * end)
    {
        uint64_t * next = ???;
    };

protected:
    // --- Define all the possible pointers to the RSQF implementations ---
    QuotientFilter<uint_size-1, 1> * qf_1;
    QuotientFilter<uint_size-2, 2> * qf_2;
    QuotientFilter<uint_size-3, 3> * qf_3;
    QuotientFilter<uint_size-4, 4> * qf_4;
    QuotientFilter<uint_size-5, 5> * qf_5;
    QuotientFilter<uint_size-6, 6> * qf_6;
    QuotientFilter<uint_size-7, 7> * qf_7;
    QuotientFilter<uint_size-8, 8> * qf_8;
    QuotientFilter<uint_size-9, 9> * qf_9;
    QuotientFilter<uint_size-10, 10> * qf_10;
    QuotientFilter<uint_size-11, 11> * qf_11;
    QuotientFilter<uint_size-12, 12> * qf_12;
    QuotientFilter<uint_size-13, 13> * qf_13;
    QuotientFilter<uint_size-14, 14> * qf_14;
    QuotientFilter<uint_size-15, 15> * qf_15;
    QuotientFilter<uint_size-16, 16> * qf_16;
    QuotientFilter<uint_size-17, 17> * qf_17;
    QuotientFilter<uint_size-18, 18> * qf_18;
    QuotientFilter<uint_size-19, 19> * qf_19;
    QuotientFilter<uint_size-20, 20> * qf_20;
    QuotientFilter<uint_size-21, 21> * qf_21;
    QuotientFilter<uint_size-22, 22> * qf_22;
    QuotientFilter<uint_size-23, 23> * qf_23;
    QuotientFilter<uint_size-24, 24> * qf_24;
    QuotientFilter<uint_size-25, 25> * qf_25;
    QuotientFilter<uint_size-26, 26> * qf_26;
    QuotientFilter<uint_size-27, 27> * qf_27;
    QuotientFilter<uint_size-28, 28> * qf_28;
    QuotientFilter<uint_size-29, 29> * qf_29;
    QuotientFilter<uint_size-30, 30> * qf_30;
    QuotientFilter<uint_size-31, 31> * qf_31;
    QuotientFilter<uint_size-32, 32> * qf_32;
    QuotientFilter<uint_size-33, 33> * qf_33;
    QuotientFilter<uint_size-34, 34> * qf_34;
    QuotientFilter<uint_size-35, 35> * qf_35;
    QuotientFilter<uint_size-36, 36> * qf_36;
    QuotientFilter<uint_size-37, 37> * qf_37;
    QuotientFilter<uint_size-38, 38> * qf_38;
    QuotientFilter<uint_size-39, 39> * qf_39;
    QuotientFilter<uint_size-40, 40> * qf_40;
    QuotientFilter<uint_size-41, 41> * qf_41;
    QuotientFilter<uint_size-42, 42> * qf_42;
    QuotientFilter<uint_size-43, 43> * qf_43;
    QuotientFilter<uint_size-44, 44> * qf_44;
    QuotientFilter<uint_size-45, 45> * qf_45;
    QuotientFilter<uint_size-46, 46> * qf_46;
    QuotientFilter<uint_size-47, 47> * qf_47;
    QuotientFilter<uint_size-48, 48> * qf_48;
    QuotientFilter<uint_size-49, 49> * qf_49;
    QuotientFilter<uint_size-50, 50> * qf_50;
    QuotientFilter<uint_size-51, 51> * qf_51;
    QuotientFilter<uint_size-52, 52> * qf_52;
    QuotientFilter<uint_size-53, 53> * qf_53;
    QuotientFilter<uint_size-54, 54> * qf_54;
    QuotientFilter<uint_size-55, 55> * qf_55;
    QuotientFilter<uint_size-56, 56> * qf_56;
    QuotientFilter<uint_size-57, 57> * qf_57;
    QuotientFilter<uint_size-58, 58> * qf_58;
    QuotientFilter<uint_size-59, 59> * qf_59;
    QuotientFilter<uint_size-60, 60> * qf_60;
    QuotientFilter<uint_size-61, 61> * qf_61;
    QuotientFilter<uint_size-62, 62> * qf_62;
    QuotientFilter<uint_size-63, 63> * qf_63;
};

#endif