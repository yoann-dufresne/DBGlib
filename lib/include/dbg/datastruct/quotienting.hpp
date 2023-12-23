#include <cstdint>

#ifndef QUOTIENTING_H
#define QUOTIENTING_H

template<uint64_t q, uint64_t r>
struct QR
{
    const uint64_t quotient : q;
    const uint64_t rest     : r;

    QR(uint64_t quotient, uint64_t rest) : quotient(quotient), rest(rest) {};
};


template<uint64_t q, uint64_t r>
QR<q,r> left_quotienting(uint64_t value)
{
    return QR<q, r>(value >> r, value);
};

template<uint64_t q, uint64_t r>
QR<q,r> right_quotienting(uint64_t value)
{
    return QR<q, r>(value, value >> q);
};

#endif