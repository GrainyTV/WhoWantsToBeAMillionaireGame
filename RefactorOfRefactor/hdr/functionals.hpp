#pragma once
#include <algorithm>
#include <numeric>
#include <type_traits>
#include <vector>

template<typename T>
concept IEnumerable = std::ranges::range<T>;

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
        return [act](auto&& container)
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
        return [act](auto&& container)
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

    template<typename Transform>
    auto mapi(Transform transform)
    {
        return [transform](const auto& container)
        {
            using ItemType = ItemOf<std::decay_t<decltype(container)>>;
            using ResultType = std::invoke_result_t<Transform, ItemType, size_t>;
            std::vector<ResultType> result;
            size_t i = 0;
            
            for (const auto& item : container)
            {
                result.push_back(transform(item, i));
                ++i;
            }
            
            return result;
        };
    }

    inline auto sum()
    {
        return [](const auto& container)
        {
            using ItemType = ItemOf<std::decay_t<decltype(container)>>;
            ItemType total = 0;
            
            for (const auto& item : container)
            {
                total += item;
            }
            
            return total;
        };
    }

    inline auto pairwise()
    {
        return [](const auto& container)
        {
            using ItemType = ItemOf<std::decay_t<decltype(container)>>;
            const size_t count = std::distance(std::begin(container), std::end(container));
            std::vector<std::pair<ItemType, ItemType>> result;
            result.reserve(count);

            for (size_t i = 0; i < count; ++i)
            {
                size_t j = (i + 1) % count;
                result.emplace_back(container[i], container[j]);
            }

            return result;
        };
    }

    inline auto flatten()
    {
        return [](const auto& container)
        {
            using InnerContainerType = ItemOf<std::decay_t<decltype(container)>>;
            using ItemType = ItemOf<std::decay_t<InnerContainerType>>;
            std::vector<ItemType> result;

            for (const auto& inner : container)
            {
                for (const auto& item : inner)
                {
                    result.emplace_back(item);
                }
            }

            return result;
        };
    }

    template<typename Predicate>
    auto forall(Predicate pred)
    {
        return [pred](const auto& container)
        {
            for (const auto& item : container)
            {
                if (pred(item) == false)
                {
                    return false;
                }
            }

            return true;
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
