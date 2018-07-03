#if !defined(DFLIB_TYPES)
#define DFLIB_TYPES

#include <type_traits>

namespace df {
    template <typename Type>
    struct is_char;

    template <typename Type, template <typename> typename... Traits>
    struct has_traits;

    template <typename First, typename Second>
    struct is_roughly_same;

    template <typename Type, template <typename> typename... Types>
    struct is_any_of;

    template <typename Type>
    struct remove_pointers;

    template <typename Type>
    class is_container;

    template <typename Type>
    struct iterator_of;

    template <typename Type>
    constexpr const bool is_char_v = is_char<Type>::value;

    template <typename Type, template <typename> typename... Traits>
    constexpr bool has_traits_v = has_traits<Type, Traits...>::value;

    template <typename First, typename Second>
    constexpr bool is_roughly_same_v = is_roughly_same<First, Second>::value;

    template <typename Type, template <typename> typename... Types>
    constexpr bool is_any_of_v = is_any_of<Type, Types...>::value;

    template <typename Type>
    constexpr bool is_container_v = is_container<Type>::value;

    template <typename Type>
    using remove_pointers_t = typename remove_pointers<Type>::type;

    template <typename Type>
    using iterator_of_t = typename iterator_of<Type>::type;
}

#define df_reflect_method(name, suffix, ...) \
    __df_reflect_method(name, suffix, __VA_ARGS__)

#define df_reflect_member(name) \
    __df_reflect_member(name)

#define df_create_trait(name, ...) \
    __df_create_trait(name, __VA_ARGS__)

#define df_dummy_v \
    std::declval<df_dummy_t>()

    // ------------------------------------------------------------------ //

namespace df {
    namespace helpers {
        template <typename... Expr>
        struct evaluable : std::is_void<std::void_t<Expr...>> { };

        template <typename Type>
        constexpr bool has_traits() {
            return true;
        }

        template <typename Type,
                  template <typename> typename Trait,
                  template <typename> typename... Rest>
        constexpr bool has_traits() {
            return Trait<Type>::value && has_traits<Type, Rest...>();
        }

        template <typename Type>
        constexpr bool is_any_of() {
            return false;
        }

        template <typename Type,  typename First, typename... Rest>
        constexpr bool is_any_of() {
            using NoRef = typename std::remove_reference<Type>::type;
            using Raw = typename std::remove_cv<NoRef>::type;
            return std::is_same_v<Raw, First> || is_any_of<Type, Rest...>();
        }
    }

    template <typename Type>
    struct is_char : std::false_type { };

    template <>
    struct is_char<char> : std::true_type { };

    template <>
    struct is_char<wchar_t> : std::true_type { };

    template <>
    struct is_char<char16_t> : std::true_type { };

    template <>
    struct is_char<char32_t> : std::true_type { };

    template <typename Type, template <typename> typename... Traits>
    struct has_traits :
        public std::bool_constant<helpers::has_traits<Type, Traits...>()> { };

    template <typename Type, template <typename> typename... Types>
    struct is_any_of :
        public std::bool_constant<helpers::is_any_of<Type, Types...>()> { };

    template <typename First, typename Second>
    struct is_roughly_same :
        std::is_same<typename std::decay<First>::type,
                     typename std::decay<Second>::type> { };

    template <typename Type>
    class is_container {
        template <typename C>
        static auto check(C&&) ->
            helpers::evaluable<decltype(std::declval<C>().begin()),
                               decltype(std::declval<C>().end())>;
        static auto check(...) -> std::false_type;
    public:
        static constexpr const bool value =
            decltype(check(std::declval<Type>()))::value;
    };

    template <typename Type>
    struct remove_pointers {
        using type = Type;
    };

    template <typename Type>
    struct remove_pointers<Type *> {
        using type = typename remove_pointers<Type>::type;
    };

    template <typename Type>
    struct iterator_of {
        template <typename C>
        static auto iterator_check(C) -> decltype(std::declval<C>().begin());
        static auto iterator_check(...) -> void;

        using type =
            typename std::conditional<
                std::is_array<Type>::value,
                typename std::decay<Type>::type,
                typename std::conditional<
                    is_container_v<Type>,
                    decltype(iterator_check(std::declval<Type>())),
                    void>::type>::type;

        static constexpr const bool valid = !std::is_void<type>::value;
    };
}

#if defined(__df_reflect_method)
#undef __df_reflect_method
#endif
#define __df_reflect_method(name, suffix, ...)                                 \
namespace df {                                                                 \
template <typename T>                                                          \
class has_method_##name##_##suffix {                                           \
    template <typename C>                                                      \
    static constexpr auto check(C) ->                                          \
        evaluable<decltype(std::declval<C>().name(__VA_ARGS__))>;              \
    static constexpr std::false_type check(...);                               \
                                                                               \
public:                                                                        \
    static constexpr bool value =                                              \
        decltype(check(std::declval<T>()))::value;                             \
};                                                                             \
                                                                               \
template <typename T, typename = void>                                         \
struct is_##name##_const_method_##suffix : std::false_type { };                \
                                                                               \
template <typename T>                                                          \
struct is_##name##_const_method_##suffix<T,                                    \
    std::void_t<decltype(std::declval<const T&>().name(__VA_ARGS__))>> :       \
        std::true_type { };                                                    \
                                                                               \
template <typename T, typename I, typename... A>                               \
bool call_##name##_##suffix(T& output, I& instance, A... args) {               \
    if constexpr (!has_method_##name##_##suffix<I>::value) {                   \
        return false;                                                          \
    } else if constexpr (std::is_const_v<I> &&                                 \
                         !is_##name##_const_method_##suffix<I>::value) {       \
        return false;                                                          \
    } else if constexpr (!std::is_void_v<T> &&                                 \
                         !std::is_void_v<                                      \
                            typename std::decay<                               \
                                decltype(instance.name(args...))>::type>) {    \
        output = instance.name(args...);                                       \
        return true;                                                           \
    } else if constexpr (std::is_void_v<                                       \
                            typename std::decay<                               \
                                decltype(instance.name(args...))>::type>) {    \
        instance.name(args...);                                                \
        return true;                                                           \
    }                                                                          \
                                                                               \
    return false;                                                              \
}                                                                              \
}

#if defined(__df_reflect_member)
#undef __df_reflect_member
#endif
#define __df_reflect_member(name)                                              \
namespace df {                                                                 \
template <typename T>                                                          \
class has_member_##name {                                                      \
    template <typename C>                                                      \
    static constexpr auto check(C) ->                                          \
            evaluable<decltype(std::declval<C>().name)>;                       \
    static constexpr std::false_type check(...);                               \
                                                                               \
public:                                                                        \
    static constexpr bool value =                                              \
        decltype(check(std::declval<T>()))::value;                             \
};                                                                             \
                                                                               \
template <typename T, typename I>                                              \
bool get_member_##name(T& output, I& instance) {                               \
    if constexpr (!has_member_##name<I>::value) {                              \
        return false;                                                          \
    } else {                                                                   \
        output = instance.name;                                                \
        return true;                                                           \
    }                                                                          \
}                                                                              \
                                                                               \
template <typename T, typename I>                                              \
bool set_member_##name(T& input, I& instance) {                                \
    if constexpr (!has_member_##name<I>::value || std::is_const_v<I>) {        \
        return false;                                                          \
    } else {                                                                   \
        instance.name = input;                                                 \
        return true;                                                           \
    }                                                                          \
}                                                                              \
}

#if defined(__df_create_trait)
#undef __df_create_trait
#endif
#define __df_create_trait(name, ...)                                           \
namespace df {                                                                 \
template <typename T>                                                          \
class name {                                                                   \
    template <typename df_dummy_t>                                             \
    static constexpr auto check(df_dummy_t) ->                                 \
        evaluable<decltype(__VA_ARGS__)>;                                      \
    static constexpr auto check(...) ->                                        \
        std::false_type;                                                       \
public:                                                                        \
    static constexpr const bool value =                                        \
        decltype(check(std::declval<T>()))::value;                             \
};                                                                             \
}

#endif
