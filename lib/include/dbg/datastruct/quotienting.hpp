#include <cstdint>

template<uint64_t q, uint64_t r>
struct QR
{
    const uint64_t quotient : q;
    const uint64_t rest     : r;

    QR(uint64_t quotient, uint64_t rest) : quotient(quotient), rest(rest) {};
};