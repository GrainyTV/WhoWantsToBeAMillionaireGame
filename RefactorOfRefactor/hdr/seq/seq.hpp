#pragma once
#include <array>
#include <numeric>
#include <vector>
#include <iterator>
#include <type_traits>
#include <algorithm>
#include <filesystem>

namespace std
{
    namespace fs = std::filesystem;

    inline fs::directory_iterator begin(fs::directory_iterator it)
    {
        return fs::begin(it);
    }

    inline fs::directory_iterator end(fs::directory_iterator it)
    {
        return fs::end(it);
    }
}

template<typename, typename = void>
constexpr bool IEnumerable = false;

// ┏━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃ C-style String Literal ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━┛

template<std::size_t N>
constexpr bool IEnumerable<const char(&)[N]> = false;

// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃ Pointer Based Enumerable ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛

template<typename T>
constexpr bool IEnumerable<T, std::enable_if_t<
    std::is_array<std::remove_reference_t<T>>::value == false &&
    std::is_pointer<decltype(std::begin(std::declval<T>()))>::value &&
    std::is_pointer<decltype(std::end(std::declval<T>()))>::value>> = true;

// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃ Iterator Based Enumerable ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

template<typename T>
constexpr bool IEnumerable<T, std::enable_if_t<
    std::is_pointer<typename decltype(std::begin(std::declval<T>()))::pointer>::value &&
    std::is_pointer<typename decltype(std::end(std::declval<T>()))::pointer>::value>> = true;

// ┏━━━━━━━━━━━━━━━━┓
// ┃ C-style Arrays ┃
// ┗━━━━━━━━━━━━━━━━┛

template<typename T, std::size_t N>
constexpr bool IEnumerable<T(&)[N]> = true;

template<typename Func, typename... Args>
#if defined(_LIBCPP_VERSION)
    using ReturnValue = typename std::__invoke_of<Func, Args...>::type;
#elif defined(__GLIBCXX__)
    using ReturnValue = typename std::__invoke_result<Func, Args...>::type;
#elif defined(_MSVC_STL_VERSION)
    using ReturnValue = std::_Invoke_result_t<Func, Args...>;
#endif

template<typename T, typename = std::enable_if_t<IEnumerable<T>>, typename Closure>
constexpr auto operator|(T&& sequence, Closure&& sequenceClosure) -> ReturnValue<Closure, T>
{
    return sequenceClosure(sequence);
}

template<typename T>
using ItemOf = std::decay_t<decltype(*std::begin(std::declval<T&>()))>;

template<typename Func, typename... Args>
#if defined(_LIBCPP_VERSION)
    constexpr bool IsInvokable = std::__invokable<Func, Args...>::value;
#elif defined(__GLIBCXX__)
    constexpr bool IsInvokable = std::__is_invocable<Func, Args...>::value;
#elif defined(_MSVC_STL_VERSION)
    constexpr bool IsInvokable = std::_Select_invoke_traits<Func, Args...>::_Is_invocable::value;
#endif

template<typename T1, typename T2>
using Equals = std::is_same<T1, T2>;

template<typename T>
constexpr std::size_t Len(const T& sequence)
{
    return static_cast<std::size_t>(std::distance(std::begin(sequence), std::end(sequence)));
}

template<bool Invocable, typename RetVal, typename Func, typename... Args>
struct IsValidSignatureHelper : std::false_type {};

template<typename RetVal, typename Func, typename... Args>
struct IsValidSignatureHelper<true, RetVal, Func, Args...> : Equals<RetVal, ReturnValue<Func, Args...>> {};
    
template<typename RetVal, typename Func, typename... Args>
constexpr bool IsValidSignature = IsValidSignatureHelper<IsInvokable<Func, Args...>, RetVal, Func, Args...>::value;

namespace Seq
{
    namespace _impl_details
    {
        template<typename T>
        using AtLeast32BitInteger = std::conditional_t<(sizeof(T) < sizeof(int)), int, T>;

        template<typename T, typename U = ItemOf<T>>
        std::enable_if_t<std::is_integral<U>::value, AtLeast32BitInteger<U>> sum(const T& sequence)
        {
            AtLeast32BitInteger<U> baseValue = 0;
            return std::accumulate(std::begin(sequence), std::end(sequence), baseValue);
        }

        template<typename T, typename U = ItemOf<T>>
        std::enable_if_t<std::is_same<U, float>::value, U> sum(const T& sequence)
        {
            return std::accumulate(std::begin(sequence), std::end(sequence), 0.0f);
        }

        template<typename T, typename U = ItemOf<T>>
        std::enable_if_t<std::is_same<U, double>::value, U> sum(const T& sequence)
        {
            return std::accumulate(std::begin(sequence), std::end(sequence), 0.0);
        }
    }

    template<typename Action>
    constexpr auto iter(Action&& action)
    {
        return [action](const auto& sequence) -> void
        {
            using T = ItemOf<decltype(sequence)>;
            static_assert(IsValidSignature<void, Action, const T&>, "Signature of Action should be <void(const T&)>");
            
            std::for_each(std::begin(sequence), std::end(sequence), action);
        };
    }

    template<typename Action>
    auto iteri(Action&& action)
    {
        return [action](const auto& sequence) -> void
        {
            using T = ItemOf<decltype(sequence)>;
            static_assert(IsValidSignature<void, Action, const T&, std::size_t>, "Signature of Action(I) should be <void(const T&, size_t)>");

            std::size_t i = 0;
            
            std::for_each(std::begin(sequence), std::end(sequence), [&action, &i](const T& item)
            {
                action(item, i);
                ++i;
            });
        };
    }

    template<typename Predicate>
    auto filter(Predicate&& predicate)
    {
        return [predicate](const auto& sequence) -> std::vector<ItemOf<decltype(sequence)>>
        {
            using T = ItemOf<decltype(sequence)>;
            static_assert(IsValidSignature<bool, Predicate, const T&>, "Signature of Predicate should be <bool(const T&)>");

            std::vector<T> result;
            result.reserve(Len(sequence));
            
            std::copy_if(std::begin(sequence), std::end(sequence), std::back_inserter(result), predicate);
            return result;
        };
    }

    inline auto sum()
    {
        return [](const auto& sequence) -> auto
        {
            using T = ItemOf<decltype(sequence)>;
            static_assert(std::is_integral<T>::value || Equals<T, float>::value || Equals<T, double>::value, "Unsupported T type for Seq::sum");

            return _impl_details::sum(sequence);
        };
    }

    template<std::size_t N, typename Mapping>
    constexpr auto init(Mapping&& mapping)
    {
        static_assert(IsInvokable<Mapping, std::size_t>, "Signature of Mapping should be <T(size_t)>");
        using T = ReturnValue<Mapping, std::size_t>;

        std::array<std::size_t, N> indices;
        std::iota(std::begin(indices), std::end(indices), 0);

        std::array<T, N> result;
        std::transform(std::begin(indices), std::end(indices), std::begin(result), mapping);

        return result;
    }

    template<int InclusiveMin, int ExclusiveMax>
    constexpr auto range()
    {
        static_assert(InclusiveMin < ExclusiveMax, "Cannot form range from expression");

        return Seq::init<ExclusiveMax - InclusiveMin>([](std::size_t i) { return InclusiveMin + static_cast<int>(i); });
    }

    template<typename Predicate>
    constexpr auto forall(Predicate&& predicate)
    {
        return [predicate](const auto& sequence) -> bool
        {
            using T = ItemOf<decltype(sequence)>;
            static_assert(IsValidSignature<bool, Predicate, const T&>, "Signature of Predicate should be <bool(const T&)>");

            return std::all_of(std::begin(sequence), std::end(sequence), predicate);
        };
    }

    template<typename Mapping>
    constexpr auto map(Mapping&& mapping)
    {
        return [mapping](const auto& sequence) -> auto
        {
            using T = ItemOf<decltype(sequence)>;
            static_assert(IsInvokable<Mapping, const T&>, "Signature of Mapping should be <U(const T&)>");
            
            using U = ReturnValue<Mapping, const T&>;

            std::vector<U> result;
            result.reserve(Len(sequence));

            std::transform(std::begin(sequence), std::end(sequence), std::back_inserter(result), mapping);
            return result;
        };
    }

    template<typename Mapping>
    constexpr auto mapi(Mapping&& mapping)
    {
        return [mapping](const auto& sequence) -> auto
        {
            using T = ItemOf<decltype(sequence)>;
            static_assert(IsInvokable<Mapping, const T&, std::size_t>, "Signature of Mapping(I) should be <U(const T&, size_t)>");
            
            using U = ReturnValue<Mapping, const T&, std::size_t>;

            std::vector<U> result;
            result.reserve(Len(sequence));

            std::size_t i = 0;

            std::transform(std::begin(sequence), std::end(sequence), std::back_inserter(result), [&mapping, &i](const T& item)
            {
                return mapping(item, i++);
            });
            
            return result;
        };
    }

    inline auto pairwise()
    {
        return [](const auto& sequence) -> auto
        {
            using T = ItemOf<decltype(sequence)>;

            std::size_t pairCount = Len(sequence) - 1;
            std::vector<std::pair<T, T>> result;
            result.reserve(pairCount);

            std::transform(
                std::begin(sequence),
                std::end(sequence) - 1,
                std::begin(sequence) + 1,
                std::back_inserter(result),
                [](const T& former, const T& latter) { return std::make_pair(former, latter); }
            );

            return result;
        };
    }

    inline auto pairwiseWrap()
    {
        return [](const auto& sequence) -> auto
        {
            using T = ItemOf<decltype(sequence)>;

            std::size_t pairCount = Len(sequence);
            std::vector<std::pair<T, T>> result(pairCount);

            auto basePairs = Seq::pairwise()(sequence);
            auto firstPair = basePairs.front();
            auto lastPair = basePairs.back();

            result.back() = std::make_pair(lastPair.second, firstPair.first);
            std::move(std::begin(basePairs), std::end(basePairs), std::begin(result));

            return result;
        };
    }

    template<typename NextSeq, typename = std::enable_if_t<IEnumerable<NextSeq>>>
    auto append(NextSeq&& nSeq)
    {
        using U = ItemOf<NextSeq>;

        return [&nSeq](const auto& sequence) -> auto
        {
            using T = ItemOf<decltype(sequence)>;
            static_assert(Equals<T, U>::value, "Inner types of sequences are not equivalent");

            std::size_t firstSequenceLength = Len(sequence);
            std::size_t secondSequenceLength = Len(nSeq);

            std::vector<T> result(firstSequenceLength + secondSequenceLength);
            std::copy(std::begin(sequence), std::end(sequence), std::begin(result));
            std::copy(std::begin(nSeq), std::end(nSeq), std::begin(result) + firstSequenceLength);

            return result;
        };
    }

    template<typename T>
    auto singleton(const T& value)
    {
        return Seq::init<1>([&value](std::size_t) { return value; });
    }
}
