#include <cstdlib>
#include <dbg/datastruct/RSQF.h>


#ifndef RSQF_MNG_H
#define RSQF_MNG_H

template<uint64_t uint_size>
class QF
{
public:
    uint64_t current_q;

    QF() : current_q (6)
    {
        qf_6 = new QuotientFilter<6, uint_size-6>();
    };

    ~QF()
    {
        switch (current_q)
        {
            case 6: delete qf_6; break;
            case 7: delete qf_7; break;
            case 8: delete qf_8; break;
            case 9: delete qf_9; break;
            case 10: delete qf_10; break;
            case 11: delete qf_11; break;
            case 12: delete qf_12; break;
            case 13: delete qf_13; break;
            case 14: delete qf_14; break;
            case 15: delete qf_15; break;
            case 16: delete qf_16; break;
            case 17: delete qf_17; break;
            case 18: delete qf_18; break;
            case 19: delete qf_19; break;
            case 20: delete qf_20; break;
            case 21: delete qf_21; break;
            case 22: delete qf_22; break;
            case 23: delete qf_23; break;
            case 24: delete qf_24; break;
            case 25: delete qf_25; break;
            case 26: delete qf_26; break;
            case 27: delete qf_27; break;
            case 28: delete qf_28; break;
            case 29: delete qf_29; break;
            case 30: delete qf_30; break;
            case 31: delete qf_31; break;
            case 32: delete qf_32; break;
            case 33: delete qf_33; break;
            case 34: delete qf_34; break;
            case 35: delete qf_35; break;
            case 36: delete qf_36; break;
            case 37: delete qf_37; break;
            case 38: delete qf_38; break;
            case 39: delete qf_39; break;
            case 40: delete qf_40; break;
            case 41: delete qf_41; break;
            case 42: delete qf_42; break;
            case 43: delete qf_43; break;
            case 44: delete qf_44; break;
            case 45: delete qf_45; break;
            case 46: delete qf_46; break;
            case 47: delete qf_47; break;
            case 48: delete qf_48; break;
            case 49: delete qf_49; break;
            case 50: delete qf_50; break;
            case 51: delete qf_51; break;
            case 52: delete qf_52; break;
            case 53: delete qf_53; break;
            case 54: delete qf_54; break;
            case 55: delete qf_55; break;
            case 56: delete qf_56; break;
            case 57: delete qf_57; break;
            case 58: delete qf_58; break;
            case 59: delete qf_59; break;
            case 60: delete qf_60; break;
            case 61: delete qf_61; break;
            case 62: delete qf_62; break;
            case 63: delete qf_63; break;
            case 64: delete qf_64; break;
            case 65: delete qf_65; break;
            case 66: delete qf_66; break;
            case 67: delete qf_67; break;
            case 68: delete qf_68; break;
            case 69: delete qf_69; break;
        }
    }

    /** Insert a list of uints in the quotient filter.
     * Resize the quotient filter if needed
     * @param begin First element of the list
     * @param end Last element of the list
     **/
    void insert(const uint64_t * begin, const uint64_t * end)
    {
        uint64_t * next = begin;

        do {
            // Insert in the right QF
            switch (current_q)
            {
            case 6: next=qf_6->insert(next, end);break;
            case 7: next=qf_7->insert(next, end);break;
            case 8: next=qf_8->insert(next, end);break;
            case 9: next=qf_9->insert(next, end);break;
            case 10: next=qf_10->insert(next, end);break;
            case 11: next=qf_11->insert(next, end);break;
            case 12: next=qf_12->insert(next, end);break;
            case 13: next=qf_13->insert(next, end);break;
            case 14: next=qf_14->insert(next, end);break;
            case 15: next=qf_15->insert(next, end);break;
            case 16: next=qf_16->insert(next, end);break;
            case 17: next=qf_17->insert(next, end);break;
            case 18: next=qf_18->insert(next, end);break;
            case 19: next=qf_19->insert(next, end);break;
            case 20: next=qf_20->insert(next, end);break;
            case 21: next=qf_21->insert(next, end);break;
            case 22: next=qf_22->insert(next, end);break;
            case 23: next=qf_23->insert(next, end);break;
            case 24: next=qf_24->insert(next, end);break;
            case 25: next=qf_25->insert(next, end);break;
            case 26: next=qf_26->insert(next, end);break;
            case 27: next=qf_27->insert(next, end);break;
            case 28: next=qf_28->insert(next, end);break;
            case 29: next=qf_29->insert(next, end);break;
            case 30: next=qf_30->insert(next, end);break;
            case 31: next=qf_31->insert(next, end);break;
            case 32: next=qf_32->insert(next, end);break;
            case 33: next=qf_33->insert(next, end);break;
            case 34: next=qf_34->insert(next, end);break;
            case 35: next=qf_35->insert(next, end);break;
            case 36: next=qf_36->insert(next, end);break;
            case 37: next=qf_37->insert(next, end);break;
            case 38: next=qf_38->insert(next, end);break;
            case 39: next=qf_39->insert(next, end);break;
            case 40: next=qf_40->insert(next, end);break;
            case 41: next=qf_41->insert(next, end);break;
            case 42: next=qf_42->insert(next, end);break;
            case 43: next=qf_43->insert(next, end);break;
            case 44: next=qf_44->insert(next, end);break;
            case 45: next=qf_45->insert(next, end);break;
            case 46: next=qf_46->insert(next, end);break;
            case 47: next=qf_47->insert(next, end);break;
            case 48: next=qf_48->insert(next, end);break;
            case 49: next=qf_49->insert(next, end);break;
            case 50: next=qf_50->insert(next, end);break;
            case 51: next=qf_51->insert(next, end);break;
            case 52: next=qf_52->insert(next, end);break;
            case 53: next=qf_53->insert(next, end);break;
            case 54: next=qf_54->insert(next, end);break;
            case 55: next=qf_55->insert(next, end);break;
            case 56: next=qf_56->insert(next, end);break;
            case 57: next=qf_57->insert(next, end);break;
            case 58: next=qf_58->insert(next, end);break;
            case 59: next=qf_59->insert(next, end);break;
            case 60: next=qf_60->insert(next, end);break;
            case 61: next=qf_61->insert(next, end);break;
            case 62: next=qf_62->insert(next, end);break;
            case 63: next=qf_63->insert(next, end);break;
            case 64: next=qf_64->insert(next, end);break;
            case 65: next=qf_65->insert(next, end);break;
            case 66: next=qf_66->insert(next, end);break;
            case 67: next=qf_67->insert(next, end);break;
            case 68: next=qf_68->insert(next, end);break;
            case 69: next=qf_69->insert(next, end);break;
            }
            // Resize needed ?
            if (next != end)
                resize();
        }
        while (next != end);
    };


    void resize()
    {
        switch(current_q)
        {
        case 6: qf_7 = new QuotientFilter<7,uint_size-7>(qf_6); delete qf_6; break;
        case 7: qf_8 = new QuotientFilter<8,uint_size-8>(qf_7); delete qf_7; break;
        case 8: qf_9 = new QuotientFilter<9,uint_size-9>(qf_8); delete qf_8; break;
        case 9: qf_10 = new QuotientFilter<10,uint_size-10>(qf_9); delete qf_9; break;
        case 10: qf_11 = new QuotientFilter<11,uint_size-11>(qf_10); delete qf_10; break;
        case 11: qf_12 = new QuotientFilter<12,uint_size-12>(qf_11); delete qf_11; break;
        case 12: qf_13 = new QuotientFilter<13,uint_size-13>(qf_12); delete qf_12; break;
        case 13: qf_14 = new QuotientFilter<14,uint_size-14>(qf_13); delete qf_13; break;
        case 14: qf_15 = new QuotientFilter<15,uint_size-15>(qf_14); delete qf_14; break;
        case 15: qf_16 = new QuotientFilter<16,uint_size-16>(qf_15); delete qf_15; break;
        case 16: qf_17 = new QuotientFilter<17,uint_size-17>(qf_16); delete qf_16; break;
        case 17: qf_18 = new QuotientFilter<18,uint_size-18>(qf_17); delete qf_17; break;
        case 18: qf_19 = new QuotientFilter<19,uint_size-19>(qf_18); delete qf_18; break;
        case 19: qf_20 = new QuotientFilter<20,uint_size-20>(qf_19); delete qf_19; break;
        case 20: qf_21 = new QuotientFilter<21,uint_size-21>(qf_20); delete qf_20; break;
        case 21: qf_22 = new QuotientFilter<22,uint_size-22>(qf_21); delete qf_21; break;
        case 22: qf_23 = new QuotientFilter<23,uint_size-23>(qf_22); delete qf_22; break;
        case 23: qf_24 = new QuotientFilter<24,uint_size-24>(qf_23); delete qf_23; break;
        case 24: qf_25 = new QuotientFilter<25,uint_size-25>(qf_24); delete qf_24; break;
        case 25: qf_26 = new QuotientFilter<26,uint_size-26>(qf_25); delete qf_25; break;
        case 26: qf_27 = new QuotientFilter<27,uint_size-27>(qf_26); delete qf_26; break;
        case 27: qf_28 = new QuotientFilter<28,uint_size-28>(qf_27); delete qf_27; break;
        case 28: qf_29 = new QuotientFilter<29,uint_size-29>(qf_28); delete qf_28; break;
        case 29: qf_30 = new QuotientFilter<30,uint_size-30>(qf_29); delete qf_29; break;
        case 30: qf_31 = new QuotientFilter<31,uint_size-31>(qf_30); delete qf_30; break;
        case 31: qf_32 = new QuotientFilter<32,uint_size-32>(qf_31); delete qf_31; break;
        case 32: qf_33 = new QuotientFilter<33,uint_size-33>(qf_32); delete qf_32; break;
        case 33: qf_34 = new QuotientFilter<34,uint_size-34>(qf_33); delete qf_33; break;
        case 34: qf_35 = new QuotientFilter<35,uint_size-35>(qf_34); delete qf_34; break;
        case 35: qf_36 = new QuotientFilter<36,uint_size-36>(qf_35); delete qf_35; break;
        case 36: qf_37 = new QuotientFilter<37,uint_size-37>(qf_36); delete qf_36; break;
        case 37: qf_38 = new QuotientFilter<38,uint_size-38>(qf_37); delete qf_37; break;
        case 38: qf_39 = new QuotientFilter<39,uint_size-39>(qf_38); delete qf_38; break;
        case 39: qf_40 = new QuotientFilter<40,uint_size-40>(qf_39); delete qf_39; break;
        case 40: qf_41 = new QuotientFilter<41,uint_size-41>(qf_40); delete qf_40; break;
        case 41: qf_42 = new QuotientFilter<42,uint_size-42>(qf_41); delete qf_41; break;
        case 42: qf_43 = new QuotientFilter<43,uint_size-43>(qf_42); delete qf_42; break;
        case 43: qf_44 = new QuotientFilter<44,uint_size-44>(qf_43); delete qf_43; break;
        case 44: qf_45 = new QuotientFilter<45,uint_size-45>(qf_44); delete qf_44; break;
        case 45: qf_46 = new QuotientFilter<46,uint_size-46>(qf_45); delete qf_45; break;
        case 46: qf_47 = new QuotientFilter<47,uint_size-47>(qf_46); delete qf_46; break;
        case 47: qf_48 = new QuotientFilter<48,uint_size-48>(qf_47); delete qf_47; break;
        case 48: qf_49 = new QuotientFilter<49,uint_size-49>(qf_48); delete qf_48; break;
        case 49: qf_50 = new QuotientFilter<50,uint_size-50>(qf_49); delete qf_49; break;
        case 50: qf_51 = new QuotientFilter<51,uint_size-51>(qf_50); delete qf_50; break;
        case 51: qf_52 = new QuotientFilter<52,uint_size-52>(qf_51); delete qf_51; break;
        case 52: qf_53 = new QuotientFilter<53,uint_size-53>(qf_52); delete qf_52; break;
        case 53: qf_54 = new QuotientFilter<54,uint_size-54>(qf_53); delete qf_53; break;
        case 54: qf_55 = new QuotientFilter<55,uint_size-55>(qf_54); delete qf_54; break;
        case 55: qf_56 = new QuotientFilter<56,uint_size-56>(qf_55); delete qf_55; break;
        case 56: qf_57 = new QuotientFilter<57,uint_size-57>(qf_56); delete qf_56; break;
        case 57: qf_58 = new QuotientFilter<58,uint_size-58>(qf_57); delete qf_57; break;
        case 58: qf_59 = new QuotientFilter<59,uint_size-59>(qf_58); delete qf_58; break;
        case 59: qf_60 = new QuotientFilter<60,uint_size-60>(qf_59); delete qf_59; break;
        case 60: qf_61 = new QuotientFilter<61,uint_size-61>(qf_60); delete qf_60; break;
        case 61: qf_62 = new QuotientFilter<62,uint_size-62>(qf_61); delete qf_61; break;
        case 62: qf_63 = new QuotientFilter<63,uint_size-63>(qf_62); delete qf_62; break;
        case 63: qf_64 = new QuotientFilter<64,uint_size-64>(qf_63); delete qf_63; break;
        case 64: qf_65 = new QuotientFilter<65,uint_size-65>(qf_64); delete qf_64; break;
        case 65: qf_66 = new QuotientFilter<66,uint_size-66>(qf_65); delete qf_65; break;
        case 66: qf_67 = new QuotientFilter<67,uint_size-67>(qf_66); delete qf_66; break;
        case 67: qf_68 = new QuotientFilter<68,uint_size-68>(qf_67); delete qf_67; break;
        case 68: qf_69 = new QuotientFilter<69,uint_size-69>(qf_68); delete qf_68; break;
        }

        current_q += 1;
    };


protected:
    // --- Define all the possible pointers to the RSQF implementations ---
    QuotientFilter<6, uint_size-6> * qf_6;
    QuotientFilter<7, uint_size-7> * qf_7;
    QuotientFilter<8, uint_size-8> * qf_8;
    QuotientFilter<9, uint_size-9> * qf_9;
    QuotientFilter<10, uint_size-10> * qf_10;
    QuotientFilter<11, uint_size-11> * qf_11;
    QuotientFilter<12, uint_size-12> * qf_12;
    QuotientFilter<13, uint_size-13> * qf_13;
    QuotientFilter<14, uint_size-14> * qf_14;
    QuotientFilter<15, uint_size-15> * qf_15;
    QuotientFilter<16, uint_size-16> * qf_16;
    QuotientFilter<17, uint_size-17> * qf_17;
    QuotientFilter<18, uint_size-18> * qf_18;
    QuotientFilter<19, uint_size-19> * qf_19;
    QuotientFilter<20, uint_size-20> * qf_20;
    QuotientFilter<21, uint_size-21> * qf_21;
    QuotientFilter<22, uint_size-22> * qf_22;
    QuotientFilter<23, uint_size-23> * qf_23;
    QuotientFilter<24, uint_size-24> * qf_24;
    QuotientFilter<25, uint_size-25> * qf_25;
    QuotientFilter<26, uint_size-26> * qf_26;
    QuotientFilter<27, uint_size-27> * qf_27;
    QuotientFilter<28, uint_size-28> * qf_28;
    QuotientFilter<29, uint_size-29> * qf_29;
    QuotientFilter<30, uint_size-30> * qf_30;
    QuotientFilter<31, uint_size-31> * qf_31;
    QuotientFilter<32, uint_size-32> * qf_32;
    QuotientFilter<33, uint_size-33> * qf_33;
    QuotientFilter<34, uint_size-34> * qf_34;
    QuotientFilter<35, uint_size-35> * qf_35;
    QuotientFilter<36, uint_size-36> * qf_36;
    QuotientFilter<37, uint_size-37> * qf_37;
    QuotientFilter<38, uint_size-38> * qf_38;
    QuotientFilter<39, uint_size-39> * qf_39;
    QuotientFilter<40, uint_size-40> * qf_40;
    QuotientFilter<41, uint_size-41> * qf_41;
    QuotientFilter<42, uint_size-42> * qf_42;
    QuotientFilter<43, uint_size-43> * qf_43;
    QuotientFilter<44, uint_size-44> * qf_44;
    QuotientFilter<45, uint_size-45> * qf_45;
    QuotientFilter<46, uint_size-46> * qf_46;
    QuotientFilter<47, uint_size-47> * qf_47;
    QuotientFilter<48, uint_size-48> * qf_48;
    QuotientFilter<49, uint_size-49> * qf_49;
    QuotientFilter<50, uint_size-50> * qf_50;
    QuotientFilter<51, uint_size-51> * qf_51;
    QuotientFilter<52, uint_size-52> * qf_52;
    QuotientFilter<53, uint_size-53> * qf_53;
    QuotientFilter<54, uint_size-54> * qf_54;
    QuotientFilter<55, uint_size-55> * qf_55;
    QuotientFilter<56, uint_size-56> * qf_56;
    QuotientFilter<57, uint_size-57> * qf_57;
    QuotientFilter<58, uint_size-58> * qf_58;
    QuotientFilter<59, uint_size-59> * qf_59;
    QuotientFilter<60, uint_size-60> * qf_60;
    QuotientFilter<61, uint_size-61> * qf_61;
    QuotientFilter<62, uint_size-62> * qf_62;
    QuotientFilter<63, uint_size-63> * qf_63;
    QuotientFilter<64, uint_size-64> * qf_64;
    QuotientFilter<65, uint_size-65> * qf_65;
    QuotientFilter<66, uint_size-66> * qf_66;
    QuotientFilter<67, uint_size-67> * qf_67;
    QuotientFilter<68, uint_size-68> * qf_68;
    QuotientFilter<69, uint_size-69> * qf_69;
};

#endif