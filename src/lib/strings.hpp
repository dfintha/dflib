#if !defined(DFLIB_STRINGS)
#define DFLIB_STRINGS

#include <locale>
#include <string>

namespace df {
    template <typename String, typename Char>
    size_t rfind_first_not_of(const String& self, const Char *values);

    template <typename String, typename Char>
    size_t rfind_last_not_of(const String& self, const Char *values);

    template <typename String, typename Char>
    String trim_front(const String& string, const Char *values);

    template <typename String, typename Char>
    String trim_back(const String& string, const Char *values);

    template <typename String, typename Char>
    String trim(const String& string, const Char *values);

    template <typename Char>
    void raw_filter(Char *string, const Char value);

    template <typename Traits>
    struct case_insensitive {
        using char_type = typename Traits::char_type;
        using int_type = typename Traits::int_type;
        using off_type = typename Traits::off_type;
        using pos_type = typename Traits::pos_type;
        using state_type = typename Traits::state_type;
        using Char = char_type;
        using Int = int_type;

        static Char toupper(Char value);
        static constexpr void assign(Char& to, const Char& from);
        static Char * move(Char *to, const Char *from, size_t n);
        static Char * copy(Char *to, const Char *from, size_t n);
        static constexpr int compare(const Char *s1, const Char *s2, size_t n);
        static constexpr size_t length(const Char *string);
        static constexpr const Char * find(const Char *s, size_t n,
                                           const Char& c);
        static constexpr Char to_char_type(Int value) noexcept;
        static constexpr Int to_int_type(Char value) noexcept;
        static constexpr bool eq_int_type(Int lhs, Int rhs) noexcept;
        static constexpr Int eof() noexcept;
        static constexpr Int not_eof(Int value) noexcept;
        static constexpr bool eq(Char lhs, Char rhs) noexcept;
        static constexpr bool lt(Char lhs, Char rhs) noexcept;
    };

    template <typename Char>
    using ci_char_traits = df::case_insensitive<std::char_traits<Char>>;

    template <typename Char>
    using ci_basic_string =
        std::basic_string<Char, ci_char_traits<Char>, std::allocator<Char>>;

    using ci_string = ci_basic_string<char>;
    using ci_wstring = ci_basic_string<wchar_t>;
    using ci_u16string = ci_basic_string<char16_t>;
    using ci_u32string = ci_basic_string<char32_t>;

    // ------------------------------------------------------------------ //

    template <typename String, typename Char>
    size_t rfind_first_not_of(const String& self, const Char *values) {
        static_assert(std::is_same_v<typename String::value_type, Char>);
        using Traits = std::char_traits<Char>;
        const size_t length = Traits::length(values);
        for (size_t i = self.length() - 1; i >= 0; --i) {
            if (Traits::find(values, length, self[i]) == nullptr)
                return i;
        }
        return String::npos;
    }

    template <typename String, typename Char>
    size_t rfind_last_not_of(const String& self, const Char *values) {
        static_assert(std::is_same_v<typename String::value_type, Char>);
        using Traits = std::char_traits<Char>;
        const size_t length = Traits::length(values);
        for (size_t i = self.length() - 1; i >= 0; --i) {
            if (!Traits::find(values, length, self[i]) == nullptr)
                return i;
        }
        return String::npos;
    }

    template <typename String, typename Char>
    String trim_front(const String& string, const Char *values) {
        static_assert(std::is_same_v<typename String::value_type, Char>);
        const size_t front = string.find_first_not_of(values);
        const size_t cut = front == std::basic_string<Char>::npos ? 0 : front;
        return string.substr(cut);
    }

    template <typename String, typename Char>
    String trim_back(const String& string, const Char *values) {
        static_assert(std::is_same_v<typename String::value_type, Char>);
        const size_t cut = rfind_first_not_of(string, values) + 1;
        return string.substr(0, cut);
    }

    template <typename String, typename Char>
    String trim(const String& string, const Char *values) {
        static_assert(std::is_same_v<typename String::value_type, Char>);
        const size_t front = string.find_first_not_of(values);
        const size_t back_cut = rfind_first_not_of(string, values) + 1;
        const size_t front_cut = front == String::npos ? 0 : front;
        return string.substr(front_cut, (back_cut - front_cut));
    }

    template <typename Char>
    void raw_filter(Char *string, const Char value) {
        Char *ptr;
        for (ptr = string; (*ptr = *string++); ptr += *ptr != value);
    }

// These are defined temporarily, so the functions can stay readable.
#define Char typename Traits::char_type
#define Int typename Traits::int_type

    template <typename Traits>
    Char case_insensitive<Traits>::toupper(Char value) {
        using ctype = std::ctype<Char>;
        return std::use_facet<ctype>(std::locale()).toupper(value);
    }

    template <typename Traits>
    constexpr void case_insensitive<Traits>::assign(Char& to,
                                                    const Char& from) {
        Traits::assign(to, from);
    }

    template <typename Traits>
    Char * case_insensitive<Traits>::move(Char *to, const Char *from,
                                          size_t n) {
        return Traits::move(to, from, n);
    }

    template <typename Traits>
    Char * case_insensitive<Traits>::copy(Char *to, const Char *from,
                                          size_t n) {
        return Traits::copy(to, from, n);
    }

    template <typename Traits>
    constexpr int case_insensitive<Traits>::compare(const Char *s1,
                                                    const Char *s2,
                                                    size_t n) {
        for (size_t i = 0; i < n; ++i) {
            const Char c1 = toupper(s1[i]);
            const Char c2 = toupper(s2[i]);
            if (Traits::lt(c1, c2)) return -1;
            if (!Traits::eq(c1, c2)) return 1;
        }
        return 0;
    }

    template <typename Traits>
    constexpr size_t case_insensitive<Traits>::length(const Char *string) {
        return Traits::length(string);
    }

    template <typename Traits>
    constexpr const Char * case_insensitive<Traits>::find(const Char *s,
                                                          size_t n,
                                                          const Char& c) {
        return Traits::find(s, n, c);
    }

    template <typename Traits>
    constexpr Char case_insensitive<Traits>::to_char_type(Int value) noexcept {
        return Traits::to_char_type(value);
    }

    template <typename Traits>
    constexpr Int case_insensitive<Traits>::to_int_type(Char value) noexcept {
        return Traits::to_int_type(value);
    }

    template <typename Traits>
    constexpr bool case_insensitive<Traits>::eq_int_type(Int lhs,
                                                         Int rhs) noexcept {
        return Traits::eq_int_type(toupper(lhs), toupper(rhs));
    }

    template <typename Traits>
    constexpr Int case_insensitive<Traits>::eof() noexcept {
        return Traits::eof();
    }

    template <typename Traits>
    constexpr Int case_insensitive<Traits>::not_eof(Int value) noexcept {
        return Traits::not_eof(value);
    }

    template <typename Traits>
    constexpr bool case_insensitive<Traits>::eq(Char lhs, Char rhs) noexcept {
        return Traits::eq(toupper(lhs), toupper(rhs));
    }

    template <typename Traits>
    constexpr bool case_insensitive<Traits>::lt(Char lhs, Char rhs) noexcept {
        return Traits::lt(toupper(lhs), toupper(rhs));
    }

#undef Char
#undef Int

}

#endif
