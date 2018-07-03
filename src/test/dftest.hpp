#if !defined(DFLIB_DFTEST)
#define DFLIB_DFTEST

#include <cfloat>
#include <cmath>
#include <iostream>
#include <functional>
#include <string>
#include <type_traits>
#include <vector>

#if defined(dftest_assert_equals)
#undef dftest_assert_equals
#endif
#define dftest_assert_equals(result, expected)                                 \
    if (!dftest::equals(result, expected)) {                                   \
        std::cout << "   Expected " << expected << ", got " << result << ".\n";\
        return false;                                                          \
    }[](){}()

#if defined(dftest_assert_true)
#undef dftest_assert_true
#endif
#define dftest_assert_true(expression)                                         \
    if (!(expression)) {                                                       \
        std::cout << "   Expression " #expression " is false.\n";              \
        return false;                                                          \
    }[](){}()

#if defined(dftest_assert_false)
#undef dftest_assert_false
#endif
#define dftest_assert_false(expression)                                        \
    if (expression) {                                                          \
        std::cout << "   Expression " #expression " is true.\n";               \
        return false;                                                          \
    }[](){}()

namespace dftest {
    struct test final {
        std::string name;
        std::function<bool()> function;
    };
    
    struct statistics final {
        unsigned total;
        unsigned passed;
        unsigned failed;
        bool success;

        statistics();
        statistics(const statistics&) = default;
        statistics(statistics&&) = default;
        statistics& operator=(const statistics&) = default;
        statistics& operator=(statistics&&) = default;
        ~statistics() = default;
    };

    class suite final {
    public:
        void add(std::string name, const std::function<bool()>& function);
        bool run();
    private:
        statistics stats;
        std::vector<test> tests;
    };

    template <typename T>
    using float_bool = 
        typename std::enable_if<std::is_floating_point<T>::value, bool>::type;

    template <typename T>
    using normal_bool = 
        typename std::enable_if<!std::is_floating_point<T>::value, bool>::type;

    template <typename T, typename U>
    inline float_bool<T> equals(const T& a, const U& b) {
        return std::fabs(a - b) <= FLT_EPSILON;
    }

    template <typename T, typename U>
    inline normal_bool<T> equals(const T& a, const U& b) {
        return a == b;
    }
}

#endif
