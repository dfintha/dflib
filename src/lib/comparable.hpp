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
        inline bool operator==(const comparable& rhs) const;
        inline bool operator!=(const comparable& rhs) const;
        inline bool operator<=(const comparable& rhs) const;
        inline bool operator>=(const comparable& rhs) const;
        inline bool operator<(const comparable& rhs) const;
        inline bool operator>(const comparable& rhs) const;
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
