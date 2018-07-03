#if !defined(DFLIB_METALIST)
#define DFLIB_METALIST

#include <iostream>
#include <type_traits>

namespace df::metalist {
    struct end {
        static constexpr const unsigned length = 0;
        using last = int;
    };

    template <typename Type, Type Value, typename Next>
    struct node {
        static constexpr const Type value = Value;
        static constexpr const unsigned length = Next::length + 1;
        using next = Next;
        using first = node;
        using last = std::conditional_t<
            std::is_same_v<next, end>, node, typename next::last>;
    };

    template <typename List>
    void print();

    template <typename List, unsigned Index>
    auto nth();

    template <typename List, typename Function>
    void for_each(Function function);

    // ------------------------------------------------------------------ //

    template <typename List>
    void print() {
        if constexpr (std::is_same_v<List, metalist::end>) {
            std::cout << '\n';
        } else {
            std::cout << List::value << ' ';
            print<typename List::next>();
        }
    }

    template <typename List, unsigned Index>
    auto nth() {
        static_assert(List::length < Index);
        if constexpr (Index == 0) {
            return List::value;
        } else {
            return nth<List::next, Index - 1>();
        }
    }

    template <typename List, typename Function>
    void for_each(Function function) {
        if constexpr (!std::is_same_v<List, end>) {
            function(List::value);
            for_each<typename List::next, Function>();
        }
    }
}

#endif
