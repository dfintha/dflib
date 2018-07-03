#if !defined(DFLIB_DEPENDANT)
#define DFLIB_DEPENDANT

#include <stdexcept>
#include <type_traits>

namespace df {
    template <typename Source, typename Result, auto Operator>
    class dependant {
    public:
        dependant(Source& source);
        dependant(const dependant&) = default;
        dependant(dependant&&) = default;
        dependant& operator=(const dependant&) = default;
        dependant& operator=(dependant&&) = default;
        ~dependant() noexcept = default;

        template <typename New>
        void attach(New& source);

        Result get() const;

    private:
        static constexpr const bool const_source = std::is_const<Source>::value;
        union {
            Source *src;
            Result res;
        } impl;
    };

    //--------------------------------------------------------------------//

    template <typename Source, typename Result, auto Operator>
    dependant<Source, Result, Operator>::dependant(Source& source) {
        attach(source);
    }

    template <typename Source, typename Result, auto Operator>
    template <typename New>
    void dependant<Source, Result, Operator>::attach(New& source) {
        static_assert(std::is_same_v<New, Source>);
        static_assert(std::is_const_v<New> == std::is_const_v<Source>);

        if constexpr (const_source) {
            impl.res = Operator(source);
        } else {
            impl.src = &source;
        }
    }

    template <typename Source, typename Result, auto Operator>
    Result dependant<Source, Result, Operator>::get() const {
        if constexpr (const_source) {
            return impl.res;
        } else {
            if (impl.src == nullptr)
                throw std::invalid_argument("invalid source for df::dependant");
            return Operator(*impl.src);
        }
    }
}

#endif
