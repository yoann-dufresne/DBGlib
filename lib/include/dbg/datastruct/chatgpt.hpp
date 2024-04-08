#include <cstdlib>
#include <memory>
#include <cassert>

template<uint64_t Q, uint64_t R>
class QuotientFilter {
public:
    // Implement QuotientFilter functionality here
    // Placeholder for compilation
    uint64_t* insert(const uint64_t* begin, const uint64_t* end) {
        // Dummy implementation
        return const_cast<uint64_t*>(end);
    }
};

template<uint64_t uint_size>
class QF {
    static_assert(uint_size > 6, "uint_size must be greater than 6 to ensure space for R.");
public:
    QF() : current_q(6), qf(createFilter(6)) {}

    ~QF() = default;

    void insert(const uint64_t* begin, const uint64_t* end) {
        uint64_t* next = begin;
        do {
            next = std::visit([&](auto&& arg) -> uint64_t* {
                return arg->insert(next, end);
            }, qf);

            if (next != end) {
                resize();
            }
        } while (next != end);
    }

private:
    uint64_t current_q;
    std::variant<
        std::unique_ptr<QuotientFilter<6, uint_size - 6>>,
        // Add more as needed up to the maximum expected Q value
        std::unique_ptr<QuotientFilter<69, uint_size - 69>>
        // Consider using a more dynamic container if the range is very large
    > qf;

    void resize() {
        assert(current_q < 69); // Adjust maximum as needed
        current_q++;
        qf = createFilter(current_q);
    }

    auto createFilter(uint64_t q) {
        switch (q) {
            case 6: return std::make_unique<QuotientFilter<6, uint_size - 6>>();
            // Repeat for other cases, up to the maximum Q value you support
            // This assumes you have a predefined set of QuotientFilter specializations
            default: throw std::runtime_error("Unsupported quotient filter size");
        }
    }
};