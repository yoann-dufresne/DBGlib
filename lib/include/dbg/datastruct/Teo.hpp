#include <cstddef>
#include <stdexcept>
#include <functional>
#include <iostream>
#include <memory>

template<
    std::size_t Max,
    std::size_t Value,
    std::size_t Step,
    typename Compare = std::equal_to<std::size_t>
>
struct runtime_dispatch
{
    static constexpr Compare compare = Compare{};

    template<
        template<std::size_t> typename Functor,
        typename... Args
    >
    static auto execute(std::size_t value, Args&&... args)
        -> decltype(Functor<Value>()(std::forward<Args>(args)...))
    {
        if (compare(value, Value))
        {
            return Functor<Value>()(std::forward<Args>(args)...);
        }

        return runtime_dispatch<Max, Value + Step, Step, Compare>::template execute<Functor, Args...>(
            value, std::forward<Args>(args)...
        );
    }
};

template<
    std::size_t Max,
    std::size_t Step,
    typename Compare
>
struct runtime_dispatch<Max, Max, Step, Compare>
{
    static constexpr Compare compare = Compare{};

    template<
        template<std::size_t> typename Functor, 
        typename... Args
    >
    static auto execute(std::size_t value, Args&&... args)
        -> decltype(Functor<Max>()(std::forward<Args>(args)...))
    {
        if (compare(value, Max))
        {
            return Functor<Max>()(std::forward<Args>(args)...);
        }
        else
        {
            throw std::runtime_error("No implementation found.");
        }
    }
};


template<typename T>
class QFBuildInterface
{
    public:
    // Ici j'ai un T* pour simplifier, mais ça peut être un kmer_iterator par exemple, qui découpe un fasta
    // en kmer au fur et à mesure.
    virtual void construct(T* begin, T* end) = 0;
};

template<typename T, std::size_t R>
class QF : public QFBuildInterface<T>
{
    public:
    QF(std::size_t size) {}

    virtual void construct(T* begin, T* end)
    {
        for (; begin != end; ++begin)
            insert(*begin);
    }

    void insert(const T& element)
    {

    }
};

template<typename T>
struct QFBuilderHelper
{
    template<std::size_t R>
    struct QFBuilder
    {
        std::unique_ptr<QFBuildInterface<T>> operator()(std::size_t size)
        {
            return std::make_unique<QF<T, R>>(size);
        }
    };
};

template<typename T>
auto make_qf_builder(std::size_t size, std::size_t r)
{
    return runtime_dispatch<32, 1, 1>::execute<QFBuilderHelper<T>::template QFBuilder>(r, size);
}

int main(void)
{
    auto qf_builder = make_qf_builder<int>(10000, 8);    

    int data[1024] = {};

    qf_builder->construct(data, data+1024);
}