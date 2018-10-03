#if !defined(DFLIB_DELEGATE)
#define DFLIB_DELEGATE

#include <utility>
#include <vector>

namespace df {
    template <typename Function>
    class delegate;

    template <typename Return, typename... Args>
    class delegate<Return(Args...)> {
    public:
        using return_type = Return;
        using function_type = Return(*)(Args...);
        using function_array = std::vector<function_type>;

        template <typename Type>
        using enable_if_void =
            std::enable_if_t<std::is_same_v<void, Type>,
                void>;

        template <typename Type>
        using enable_if_not_void =
            std::enable_if_t<!std::is_same_v<void, Type>,
                std::vector<return_type>>;

        delegate operator+(const function_type& rhs) const;
        delegate operator+(const delegate& rhs) const;
        void operator+=(const function_type& rhs);
        void operator+=(const delegate& rhs);

        delegate operator-(const function_type& rhs) const;
        delegate operator-(const delegate& rhs) const;
        void operator-=(const function_type& rhs);
        void operator-=(const delegate& rhs);

        const function_array& invocation_list() const;

        template <typename T = Return>
        enable_if_void<T> operator()(Args&&... arguments) const;

        template <typename T = Return>
        enable_if_not_void<T> operator()(Args&&... arguments) const;

    private:
        function_array functions;
    };

    // ------------------------------------------------------------------ //

    #define delegate delegate<Return(Args...)>
    #define function_array typename delegate::function_array

    template <typename Return, typename... Args>
    delegate delegate::operator+(const function_type& rhs) const {
        delegate copy = *this;
        copy += rhs;
        return copy;
    }

    template <typename Return, typename... Args>
    delegate delegate::operator+(const delegate& rhs) const {
        delegate copy = *this;
        copy += rhs;
        return copy;
    }

    template <typename Return, typename... Args>
    void delegate::operator+=(const function_type& rhs) {
        for (auto f : functions) {
            if (f == rhs) return;
        }
        functions.push_back(rhs);
    }

    template <typename Return, typename... Args>
    void delegate::operator+=(const delegate& rhs) {
        for (auto f : rhs.functions) {
            *this += f;
        }
    }

    template <typename Return, typename... Args>
    delegate delegate::operator-(const function_type& rhs) const {
        delegate copy = *this;
        copy -= rhs;
        return copy;
    }

    template <typename Return, typename... Args>
    delegate delegate::operator-(const delegate& rhs) const {
        delegate copy = *this;
        copy -= rhs;
        return copy;
    }

    template <typename Return, typename... Args>
    void delegate::operator-=(const function_type& rhs) {
        for (auto it = functions.begin(); it != functions.end(); ++it) {
            if (*it == rhs) {
                functions.erase(it);
                return;
            }
        }
    }

    template <typename Return, typename... Args>
    void delegate::operator-=(const delegate& rhs) {
        for (auto f : rhs.functions) {
            *this -= f;
        }
    }

    template <typename Return, typename... Args>
    const function_array& delegate::invocation_list () const {
        return functions;
    }

    #define enable_if_void typename delegate::template enable_if_void

    template <typename Return, typename... Args>
    template <typename T>
    enable_if_void<T> delegate::operator()(Args&&... arguments) const {
        for (const auto& f : functions) {
            f(std::forward<Args&&>(arguments)...);
        }
    }

    #undef enable_if_void
    #define enable_if_not_void typename delegate::template enable_if_not_void

    template <typename Return, typename... Args>
    template <typename T>
    enable_if_not_void<T> delegate::operator()(Args&&... arguments) const {
        std::vector<return_type> result;
        result.reserve(functions.size());
        for (const auto& f : functions) {
            result.push_back(f(std::forward<Args&&>(arguments)...));
        }
        return result;
    }

    #undef enable_if_not_void
    #undef function_array
    #undef delegate
}

#endif
