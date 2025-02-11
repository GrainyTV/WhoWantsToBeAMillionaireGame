#pragma once
#include <numeric>
#include <type_traits>
#include <algorithm>
#include <vector>

template<typename T>
concept IEnumerable = requires (const T& seq)
{
    std::begin(seq);
    std::end(seq);
};

template<IEnumerable T>
using ItemOf = typename T::value_type;

template<IEnumerable T, typename Func>
constexpr auto operator|(T&& input, Func&& func)
{
    return func(std::forward<T>(input));
}

namespace Seq
{
    template<typename Action>
    auto iter(Action act)
    {
        return [act](auto& container)
        {
            for (auto& item : container)
            {
                act(item);
            }
        };
    }

    template<typename Action>
    auto iteri(Action act)
    {
        return [act](auto& container)
        {
            size_t i = 0;

            for (auto& item : container)
            {
                act(item, i);
                ++i;
            }
        };
    }

    template<typename Predicate>
    auto filter(Predicate pred)
    {
        return [pred](const auto& container)
        {
            using ResultType = ItemOf<std::decay_t<decltype(container)>>;
            std::vector<ResultType> result;
            
            for (const auto& item : container)
            {
                if (pred(item))
                {
                    result.push_back(item);
                }
            }
            
            return result;
        };
    }

    template<typename Transform>
    auto map(Transform transform)
    {
        return [transform](const auto& container)
        {
            using ItemType = ItemOf<std::decay_t<decltype(container)>>;
            using ResultType = std::invoke_result_t<Transform, ItemType>;
            std::vector<ResultType> result;
            
            for (const auto& item : container)
            {
                result.push_back(transform(item));
            }
            
            return result;
        };
    }

    template<IEnumerable T>
    auto sum() 
    {
        return [](const T& container)
        {
            ItemOf<T> total = 0;
            
            for (const auto& item : container)
            {
                total += item;
            }
            
            return total;
        };
    }

    template<int32_t inclusiveMin, int32_t exclusiveMax>
    requires (exclusiveMax > inclusiveMin)
    consteval auto range()
    {
        std::array<int32_t, exclusiveMax - inclusiveMin> elements;
        std::iota(elements.begin(), elements.end(), inclusiveMin);
        return elements;
    }
}
