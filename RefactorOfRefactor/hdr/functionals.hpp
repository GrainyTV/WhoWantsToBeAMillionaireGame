#pragma once
#include <ranges>
#include <numeric>
#include <type_traits>
#include <algorithm>
#include <vector>

namespace stdr = std::ranges;

template<typename T>
concept IEnumerable = stdr::range<T>;

template<IEnumerable T>
using ItemOf = typename T::value_type;

template<typename Func, typename Item>
concept ForEachFunction = 
    (std::is_invocable_v<Func, const Item&> &&
     std::is_same_v<std::invoke_result_t<Func, const Item&>, void>) ||
    (std::is_invocable_v<Func, Item&> &&
     std::is_same_v<std::invoke_result_t<Func, Item&>, void>);

template<typename Func, typename Item>
concept ForEachFunctionIndex =
    (std::is_invocable_v<Func, const Item&, const size_t> &&
     std::is_same_v<std::invoke_result_t<Func, const Item&, const size_t>, void>) ||
    (std::is_invocable_v<Func, Item&, const size_t> &&
     std::is_same_v<std::invoke_result_t<Func, Item&, const size_t>, void>);

template<typename Func, typename Item>
concept FilterFunction =
    std::is_invocable_v<Func, Item> &&
    std::is_same_v<std::invoke_result_t<Func, Item>, bool>;

template<typename Func, typename Item>
concept MapFunction = std::is_invocable_v<Func, Item>;

namespace Functionals
{
    template<IEnumerable T, ForEachFunction<ItemOf<T>> Func>
    void forEach(T& container, Func&& action)
    {
        std::for_each(std::begin(container), std::end(container), action);
    }

    template<IEnumerable T, ForEachFunctionIndex<ItemOf<T>> Func>
    void forEach(T& container, Func&& action)
    {
        size_t index = 0;     
        
        std::for_each(std::begin(container), std::end(container), [&](ItemOf<T>& item)
        {
            action(item, index);
            ++index;
        });
    }

    template<IEnumerable T, FilterFunction<ItemOf<T>> Func>
    auto filter(const T& container, Func&& predicate)
    {
        std::vector<ItemOf<T>> filteredItems;
        filteredItems.reserve(container.size());

        std::copy_if(
            container.cbegin(),
            container.cend(),
            std::back_inserter(filteredItems),
            predicate);
        
        return filteredItems;
    }

    template<IEnumerable T, MapFunction<ItemOf<T>> Func>
    auto map(const T& container, Func&& transform)
    {
        using ResultType = std::invoke_result_t<Func, ItemOf<T>>;
        std::vector<ResultType> mappedItems;
        mappedItems.reserve(container.size());
        
        std::transform(
            container.cbegin(),
            container.cend(),
            std::back_inserter(mappedItems),
            transform);

        return mappedItems;
    }

    template<int32_t inclusiveMin, int32_t exclusiveMax>
    requires (exclusiveMax > inclusiveMin)
    consteval auto range()
    {
        std::array<int32_t, exclusiveMax - inclusiveMin> elements;
        std::iota(elements.begin(), elements.end(), inclusiveMin);
        return elements;
    }

    // template<typename T>
    // using ItemOf = typename std::iterator_traits<decltype(stdr::begin(std::declval<T&>()))>::value_type;

    // template<typename Func, typename T>
    // concept UnaryVoidFunction = std::invocable<Func, T, size_t> && std::is_void_v<std::invoke_result_t<Func, T, size_t>>;

    // template<typename Func, typename T>
    // concept UnaryBooleanFunction = std::invocable<Func, T> && std::is_same_v<std::invoke_result_t<Func, T>, bool>;

    // template<typename Func, typename T, typename U, typename Return>
    // concept BinaryFunction = std::invocable<Func, T, U> && std::is_same_v<std::invoke_result_t<Func, T, U>, Return>;

    // template<typename T>
    // concept IEnumerable = requires(const T& item)
    // {
    //     stdr::begin(item);
    //     stdr::end(item);
    // };

    // template<typename Container, typename Func>
    // requires IEnumerable<Container> && UnaryVoidFunction<Func, ItemOf<Container>>
    // void forEach(const Container& items, Func&& action)
    // {
    //     size_t index = 0;

    //     for (const auto& item : items)
    //     {
    //         action(item, index);
    //         ++index;
    //     }
    // }

    // template<typename Container, typename Func>
    // requires IEnumerable<Container> && UnaryBooleanFunction<Func, ItemOf<Container>>
    // auto filter(const Container& items, Func&& action)
    // {
    //     std::vector<ItemOf<Container>> filteredItems;

    //     forEach(items, [&](const auto& item, size_t /*i*/)
    //     {
    //         if (action(item))
    //         {
    //             filteredItems.push_back(item);
    //         }
    //     });

    //     return filteredItems;
    // }

    // template<typename Container, typename Func>
    // requires IEnumerable<Container>
    // auto map(const Container& items, Func&& action)
    // {
    //     using OutputValueType = std::invoke_result_t<Func, ItemOf<Container>>;
    //     std::vector<OutputValueType> mappedItems;
    //     mappedItems.reserve(std::distance(stdr::begin(items), stdr::end(items)));

    //     forEach(items, [&](const auto& item, size_t /*i*/)
    //     {
    //         mappedItems.push_back(action(item));
    //     });

    //     return mappedItems;
    // }

    // template<typename Container, typename Func, typename T>
    // requires IEnumerable<Container> && BinaryFunction<Func, T, ItemOf<Container>, T>
    // auto reduce(const Container& items, const T initial, Func&& action)
    // {
    //     T result = initial;

    //     forEach(items, [&](const auto& item, size_t /*i*/)
    //     {
    //         result = action(result, item);
    //     });

    //     return result;
    // }
}