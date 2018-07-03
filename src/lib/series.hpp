#if !defined(DFLIB_SERIES)
#define DFLIB_SERIES

#include <vector>

namespace df {
    template <typename Integer>
    Integer fibonacci(Integer index);

    template <typename Integer>
    Integer factorial(Integer index);

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

#endif
