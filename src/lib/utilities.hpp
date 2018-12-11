#if !defined(DFLIB_UTILITIES)
#define DFLIB_UTILITIES

#include <clocale>
#include <cstddef>
#include <cstring>
#include <functional>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

namespace df {
    constexpr const char nl = '\n';

    template <typename Iterator>
    Iterator max(Iterator begin, Iterator end);

    template <typename Iterator>
    Iterator min(Iterator begin, Iterator end);

    template <typename Iterator>
    std::pair<Iterator, Iterator> minmax(Iterator begin, Iterator end);

    template <typename Callable, typename... Arguments>
    inline auto noopt_invoke(const Callable& f, Arguments... args);

    template <typename... List>
    void print(List&... list);

    template <typename... List>
    void println(List&... list);

    template <typename Type>
    const std::byte * bytes_of(const Type& instance);

    template <typename Type>
    void clear(Type& instance);

    template <typename Size>
    std::string size_to_string(Size size);

    // ------------------------------------------------------------------ //

    namespace helpers {
        void print() { }

        template <typename First, typename... Rest>
        void print(First& first, Rest&... rest) {
            std::cout << first;
            print(rest...);
        }
    }

    template <typename Iterator>
    Iterator max(Iterator begin, Iterator end) {
        Iterator max = begin++;
        for (Iterator it = begin; it != end; ++it) {
            if (*it > *max) max = it;
        }
        return max;
    }

    template <typename Iterator>
    Iterator min(Iterator begin, Iterator end) {
        Iterator min = begin++;
        for (Iterator it = begin; it != end; ++it) {
            if (*it < *min) min = it;
        }
        return min;
    }

    template <typename Iterator>
    std::pair<Iterator, Iterator> minmax(Iterator begin, Iterator end) {
        Iterator min = begin;
        Iterator max = begin++;
        for (Iterator it = begin; it != end; ++it) {
            if (*it < *min) min = it;
            if (*it > *max) max = it;
        }
        return std::make_pair<min, max>;
    }

    template <typename Callable, typename... Arguments>
    inline auto noopt_invoke(const Callable& f, Arguments... args) {
        const volatile auto& function = f;
        return function(args...);
    }

    template <typename... List>
    void print(List&... list) {
        helpers::print(list...);
    }

    template <typename... List>
    void println(List&... list) {
        helpers::print(list..., nl);
    }

    template <typename Type>
    const std::byte * bytes_of(const Type& instance) {
        static_assert(!std::is_pointer<Type>::value);
        const std::byte *bytes = reinterpret_cast<const std::byte *>(&instance);
        return bytes;
    }

    template <typename Type>
    void clear(Type& instance) {
        static constexpr const bool pointer_type = std::is_pointer<Type>::value;
        static constexpr const bool pod_type = std::is_pod<Type>::value;
        static_assert(!pointer_type && pod_type);
        std::memset(&instance, 0x00, sizeof(Type));
    }

    template <typename Size>
    std::string size_to_string(Size size) {
        static_assert(std::is_integral<Size>::value, 
                      "The size must be an integer!");

        static constexpr const char prefixes[] = " KMGTPEZY";
        static constexpr const int max = sizeof(prefixes) - 1;
        static constexpr const int step = 1024;
        static char buffer[16];

        double work = double(size);
        short index = 0;
        while (work >= step && index < max) {
            work /= step;
            ++index;
        }

        const char *format = (index > 0) ? "%.2f %ciB" : "%.2f B";
        snprintf(buffer, 15, format, work, prefixes[index]);

        char *decimal = strstr(buffer, ".");
        if (decimal != nullptr) {
            std::setlocale(LC_NUMERIC, "");
            struct lconv *lc = std::localeconv();
            *decimal = *lc->decimal_point;
        }

        return std::string(buffer);
    }
}

#endif
