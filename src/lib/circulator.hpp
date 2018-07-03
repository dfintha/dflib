#if !defined(DFLIB_CIRCULATOR)
#define DFLIB_CIRCULATOR

#include <type_traits>

namespace df {
    template <typename Iterator>
    using deref_type = decltype(*std::declval<Iterator>());

    template <typename Iterator>
    using arrow_type = decltype(std::declval<Iterator>().operator->());

    template <typename Iterator>
    class circulator {
    public:
        using difference_type = typename Iterator::difference_type;
        using value_type = typename Iterator::value_type;
        using pointer = typename Iterator::pointer;
        using reference = typename Iterator::reference;
        using iterator_category = typename Iterator::iterator_category;

        circulator(Iterator beg, Iterator end);
        circulator(Iterator beg, Iterator end, Iterator at);
        circulator(const circulator& other) = default;
        circulator(circulator&& other) = default;
        circulator& operator=(const circulator& other) = default;
        circulator& operator=(circulator&& other) = default;
        ~circulator() = default;

        circulator& operator++();
        circulator operator++(int);
        circulator& operator--();
        circulator operator--(int);
        deref_type<Iterator> operator*() const;
        arrow_type<Iterator> operator->() const;

    private:
        void next();
        void previous();

        const Iterator begin_;
        const Iterator end_;
        Iterator where_;
    };

    //--------------------------------------------------------------------//

    template <typename Iterator>
    circulator<Iterator>::circulator(Iterator beg, Iterator end) :
        begin_(beg),
        end_(end),
        where_(beg)
    { }

    template <typename Iterator>
    circulator<Iterator>::circulator(Iterator beg, Iterator end, Iterator at) :
        begin_(beg),
        end_(end),
        where_(at)
    { }

    template <typename Iterator>
    circulator<Iterator>& circulator<Iterator>::operator++() {
        next();
        return *this;
    }

    template <typename Iterator>
    circulator<Iterator> circulator<Iterator>::operator++(int) {
        circulator temp(*this);
        next();
        return temp;
    }

    template <typename Iterator>
    circulator<Iterator>& circulator<Iterator>::operator--() {
        previous();
        return *this;
    }

    template <typename Iterator>
    circulator<Iterator> circulator<Iterator>::operator--(int) {
        circulator temp(*this);
        previous();
        return temp;
    }

    template <typename Iterator>
    deref_type<Iterator> circulator<Iterator>::operator*() const {
        return *where_;
    }

    template <typename Iterator>
    arrow_type<Iterator> circulator<Iterator>::operator->() const {
        return where_.operator->();
    }

    template <typename Iterator>
    void circulator<Iterator>::next() {
        ++where_;
        if (where_ == end_)
            where_ = begin_;
    }

    template <typename Iterator>
    void circulator<Iterator>::previous() {
        if (where_ == begin_)
            where_ = end_;
        --where_;
    }
}

#endif
