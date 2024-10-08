#pragma once
#include <iterator>
#include <ranges>
#include <vector>
#include <concepts>
#include <type_traits>

namespace stdr = std::ranges;

namespace Functionals
{
    template<typename T>
    using ItemOf = typename std::iterator_traits<decltype(stdr::begin(std::declval<T&>()))>::value_type;

    template<typename Func, typename T>
    concept UnaryVoidFunction = std::invocable<Func, T, size_t> && std::is_void_v<std::invoke_result_t<Func, T, size_t>>;

    template<typename Func, typename T>
    concept UnaryBooleanFunction = std::invocable<Func, T> && std::is_same_v<std::invoke_result_t<Func, T>, bool>;

    template<typename Func, typename T, typename U, typename Return>
    concept BinaryFunction = std::invocable<Func, T, U> && std::is_same_v<std::invoke_result_t<Func, T, U>, Return>;

    template<typename T>
    concept IEnumerable = requires(const T& item)
    {
        stdr::begin(item);
        stdr::end(item);
    };

    template<typename Container, typename Func>
    requires IEnumerable<Container> && UnaryVoidFunction<Func, ItemOf<Container>>
    void forEach(const Container& items, Func&& action)
    {
        size_t index = 0;

        for (const auto& item : items)
        {
            action(item, index);
            ++index;
        }
    }

    template<typename Container, typename Func>
    requires IEnumerable<Container> && UnaryBooleanFunction<Func, ItemOf<Container>>
    auto filter(const Container& items, Func&& action)
    {
        std::vector<ItemOf<Container>> filteredItems;

        forEach(items, [&](const auto& item, size_t /*i*/)
        {
            if (action(item))
            {
                filteredItems.push_back(item);
            }
        });

        return filteredItems;
    }

    template<typename Container, typename Func>
    requires IEnumerable<Container>
    auto map(const Container& items, Func&& action)
    {
        using OutputValueType = std::invoke_result_t<Func, ItemOf<Container>>;
        std::vector<OutputValueType> mappedItems;
        mappedItems.reserve(std::distance(stdr::begin(items), stdr::end(items)));

        forEach(items, [&](const auto& item, size_t /*i*/)
        {
            mappedItems.push_back(action(item));
        });

        return mappedItems;
    }

    template<typename Container, typename Func, typename T>
    requires IEnumerable<Container> && BinaryFunction<Func, T, ItemOf<Container>, T>
    auto reduce(const Container& items, const T initial, Func&& action)
    {
        T result = initial;

        forEach(items, [&](const auto& item, size_t /*i*/)
        {
            result = action(result, item);
        });

        return result;
    }
}