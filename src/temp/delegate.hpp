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
        using return_array = std::vector<return_type>;
        using function_type = Return(*)(Args...);
        using function_array = std::vector<function_type>;

        delegate operator+(const function_type& rhs) const;
        delegate operator+(const delegate& rhs) const;
        void operator+=(const function_type& rhs);
        void operator+=(const delegate& rhs);

        delegate operator-(const function_type& rhs) const;
        delegate operator-(const delegate& rhs) const;
        void operator-=(const function_type& rhs);
        void operator-=(const delegate& rhs);

        const function_array& invocation_list() const;
        return_array operator()(Args&&... arguments) const;

    private:
        function_array functions;
    };

    // ------------------------------------------------------------------ //

    #define delegate delegate<Return(Args...)>
    #define function_array typename delegate::function_array
    #define return_array typename delegate::return_array

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
        delegate& self = *this;
        for (auto f : rhs.functions) {
            self += f;
        }
    }

    // TODO: Operator -=

    template <typename Return, typename... Args>
    const function_array& delegate::invocation_list () const {
        return functions;
    }

    // TODO: EnableIf for Void Return Type

    template <typename Return, typename... Args>
    return_array delegate::operator()(Args&&... args) const {
        return_array result;
        result.reserve(functions.size());
        for (const auto& f : functions) {
            result.push_back(f(std::forward<Args&&>(args)...));
        }
        return result;
    }

    #undef delegate
    #undef function_array
    #undef return_array
}

#endif
