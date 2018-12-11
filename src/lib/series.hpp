#if !defined(DFLIB_SERIES)
#define DFLIB_SERIES

#include <vector>

namespace df {
    template <typename Integer>
    Integer fibonacci(Integer index);

    template <typename Integer>
    Integer factorial(Integer index);

#define df_create_series(name, initial_cache, expression) \
    __df_create_series(name, initial_cache, expression)

    // ------------------------------------------------------------------ //

    template <typename Integer>
    Integer fibonacci(Integer index) {
        static std::vector<Integer> values{1, 1, 1};
        if (values.size() <= size_t(index))
            values.resize(index + 1, Integer(0));

        if (values[index] == Integer(0)) {
            values[index] = fibonacci(index - 1) + fibonacci(index - 2);
        }
        return values[index];
    }

    template <typename Integer>
    Integer factorial(Integer index) {
        static std::vector<Integer> values{1, 1};
        if (values.size() <= size_t(index))
            values.resize(index + 1, Integer(0));

        if (values[index] == Integer(0))
            values[index] = index * factorial(index - 1);
        return values[index];
    }
}

#define __df_create_series(name, initial_cache, expression)                 \
namespace df {                                                              \
    template <typename Integer>                                             \
    Integer name(Integer n) {                                               \
        static std::vector<Integer> cache initial_cache;                    \
        if (cache.size() <= std::size_t(n))                                 \
            cache.resize(n + 1, Integer(0));                                \
        if (cache[n] == Integer(0))                                         \
            cache[n] = expression;                                          \
        return cache[n];                                                    \
    }                                                                       \
}

#endif
