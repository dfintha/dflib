#if !defined(DFLIB_COMPARABLE)
#define DFLIB_COMPARABLE

namespace df {
    enum comparison_result {
        lesser = -1,
        equal = 0,
        greater = 1
    };

    template <typename Type>
    class comparable {
    public:
        virtual ~comparable() noexcept = default;
        inline bool operator==(const comparable& rhs) const;
        inline bool operator!=(const comparable& rhs) const;
        inline bool operator<=(const comparable& rhs) const;
        inline bool operator>=(const comparable& rhs) const;
        inline bool operator<(const comparable& rhs) const;
        inline bool operator>(const comparable& rhs) const;

    private:
        virtual comparison_result compare(const comparable& rhs) const = 0;
    };

    //--------------------------------------------------------------------//

    template <typename Type>
    inline bool comparable<Type>::operator==(const comparable& rhs) const {
        return compare(rhs) == comparison_result::equal;
    }

    template <typename Type>
    inline bool comparable<Type>::operator!=(const comparable& rhs) const {
        return compare(rhs) != comparison_result::equal;
    }

    template <typename Type>
    inline bool comparable<Type>::operator<=(const comparable& rhs) const {
        return compare(rhs) != comparison_result::greater;
    }

    template <typename Type>
    inline bool comparable<Type>::operator>=(const comparable& rhs) const {
        return compare(rhs) != comparison_result::lesser;
    }

    template <typename Type>
    inline bool comparable<Type>::operator<(const comparable& rhs) const {
        return compare(rhs) == comparison_result::lesser;
    }

    template <typename Type>
    inline bool comparable<Type>::operator>(const comparable& rhs) const {
        return compare(rhs) == comparison_result::greater;
    }
}

#endif
