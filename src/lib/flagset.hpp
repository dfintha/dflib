#if !defined(DFLIB_FLAGSET)
#define DFLIB_FLAGSET

#include <cstdint>

namespace df {
    namespace helpers {
        template <uint8_t Bits>
        struct bits;
    }

    template <uint8_t Bits, uint8_t Index>
    constexpr const typename helpers::bits<Bits>::type nth_mask();

    template <uint8_t Bits>
    constexpr const typename helpers::bits<Bits>::type full_mask();

    template <uint8_t Bits>
    class flagset {
    public:
        using container_type = typename helpers::bits<Bits>::type;

        constexpr flagset(container_type src = 0);
        constexpr flagset(const flagset&) = default;
        constexpr flagset(flagset&&) = default;
        constexpr flagset& operator=(container_type src);
        constexpr flagset& operator=(const flagset&) = default;
        constexpr flagset& operator=(flagset&&) = default;

        template <uint8_t Index>
        void set();

        template <uint8_t Index>
        void clear();

        template <uint8_t Index>
        constexpr bool test() const;

        void reset();
        static constexpr uint8_t length();
        constexpr operator container_type();

    private:
        container_type field;
    };

    using flagset8 = flagset<8>;
    using flagset16 = flagset<16>;
    using flagset32 = flagset<32>;
    using flagset64 = flagset<64>;

    // ------------------------------------------------------------------ //

    template <uint8_t Bits, uint8_t Index>
    constexpr const typename helpers::bits<Bits>::type nth_mask() {
        static_assert(Index <= Bits && Index != 0);
        return typename helpers::bits<Bits>::type(1) << (Index - 1);
    }

    template <uint8_t Bits>
    constexpr const typename helpers::bits<Bits>::type full_mask() {
        return typename helpers::bits<Bits>::type(-1);
    }

    template <uint8_t Bits>
    constexpr flagset<Bits>::flagset(container_type src) : field(src) { }

    template <uint8_t Bits>
    constexpr flagset<Bits>& flagset<Bits>::operator=(container_type src) {
        field = src;
        return *this;
    }

    template <uint8_t Bits>
    template <uint8_t Index>
    void flagset<Bits>::set() {
        static_assert(Index <= Bits && Index != 0);
        field |= nth_mask<Bits, Index>();
    }

    template <uint8_t Bits>
    template <uint8_t Index>
    void flagset<Bits>::clear() {
        static_assert(Index <= Bits && Index != 0);
        field &= (~nth_mask<Bits, Index>());
    }

    template <uint8_t Bits>
    template <uint8_t Index>
    constexpr bool flagset<Bits>::test() const {
        static_assert(Index <= Bits && Index != 0);
        return (field & nth_mask<Bits, Index>()) != 0;
    }

    template <uint8_t Bits>
    void flagset<Bits>::reset() {
        field = 0;
    }

    template <uint8_t Bits>
    constexpr uint8_t flagset<Bits>::length() {
        return Bits;
    }

    template <uint8_t Bits>
    constexpr flagset<Bits>::operator flagset<Bits>::container_type() {
        return field;
    }

    namespace helpers {
        template <uint8_t Bits>
        struct bits {
            static_assert(Bits == 8 || Bits == 16 || Bits == 32 || Bits == 64);
            using type = uint8_t;
        };

        template <> struct bits<8> {
            using type = uint8_t;
        };

        template <> struct bits<16> {
            using type = uint16_t;
        };

        template <> struct bits<32> {
            using type = uint32_t;
        };

        template <> struct bits<64> {
            using type = uint64_t;
        };
    }
}

#endif
