#if !defined(DFLIB_NUMBER)
#define DFLIB_NUMBER

#include <cfloat>
#include <cmath>
#include <type_traits>

namespace df {
    template <typename Type>
    struct epsilon;

    template <typename Type>
    constexpr const Type epsilon_v = epsilon<Type>::value;

    template <typename Type>
    class number {
    public:
        static constexpr const bool integer = std::is_integral_v<Type>;
        static constexpr const bool floating = std::is_floating_point_v<Type>;
        static_assert(integer || floating);

        number(const Type& n = Type(0));
        number(const number&) = default;
        number(number&&) = default;
        number& operator=(const number&) = default;
        number& operator=(number&&) = default;
        number& operator=(const Type& n);

        operator Type&();
        operator const Type&() const;

    private:
        Type content;
    };

    template <typename First, typename Second>
    bool operator==(const number<First>& lhs, const number<Second>& rhs);

    template <typename Number, typename Other>
    bool operator==(const number<Number>& lhs, const Other& rhs);

    template <typename Number, typename Other>
    bool operator==(const Other& lhs, const number<Number>& rhs);

    template <typename First, typename Second>
    bool operator!=(const number<First>& lhs, const number<Second>& rhs);

    template <typename Number, typename Other>
    bool operator!=(const number<Number>& lhs, const Other& rhs);

    template <typename Number, typename Other>
    bool operator!=(const Other& lhs, const number<Number>& rhs);

    // ------------------------------------------------------------------ //

    template <typename Type>
    struct epsilon {
        static constexpr const bool integer = std::is_integral_v<Type>;
        static constexpr const bool floating = std::is_floating_point_v<Type>;
        static_assert(integer || floating);

        static constexpr const Type value = Type(0);
    };

    template <>
    struct epsilon<float> {
        static constexpr const float value = FLT_EPSILON;
    };

    template <>
    struct epsilon<double> {
        static constexpr const double value = DBL_EPSILON;
    };

    template <>
    struct epsilon<long double> {
        static constexpr const long double value = LDBL_EPSILON;
    };

    template <typename Type>
    number<Type>::number(const Type& n) : content(n) { }

    template <typename Type>
    number<Type>& number<Type>::operator=(const Type& n) {
        content = n;
    }

    template <typename Type>
    number<Type>::operator Type&() {
        return content;
    }

    template <typename Type>
    number<Type>::operator const Type&() const {
        return content;
    }

    template <typename First, typename Second>
    bool operator==(const number<First>& lhs, const number<Second>& rhs) {
        static constexpr const long double greater_epsilon =
            epsilon_v<First> > epsilon_v<Second> ?
                epsilon_v<First> : epsilon_v<Second>;
        return std::abs(First(lhs) - Second(rhs)) < greater_epsilon;
    }

    template <typename Number, typename Other>
    bool operator==(const number<Number>& lhs, const Other& rhs) {
        return number<Other>(lhs) == rhs;
    }

    template <typename Number, typename Other>
    bool operator==(const Other& lhs, const number<Number>& rhs) {
        return number<Other>(lhs) == rhs;
    }

    template <typename First, typename Second>
    bool operator!=(const number<First>& lhs, const number<Second>& rhs) {
        return !(lhs == rhs);
    }

    template <typename Number, typename Other>
    bool operator!=(const number<Number>& lhs, const Other& rhs) {
        return !(lhs == rhs);
    }

    template <typename Number, typename Other>
    bool operator!=(const Other& lhs, const number<Number>& rhs) {
        return !(lhs == rhs);
    }
}

#endif
