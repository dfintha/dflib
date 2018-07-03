#if !defined(DFLIB_DOCUMENTATION)
#define DFLIB_DOCUMENTATION

namespace df {
    template <typename Type>
    using owner_c_ptr = Type *;

    template <typename Type>
    using owner_ptr = Type *;

    template <typename Type>
    using array_owner_ptr = Type *;

    template <typename Type>
    using observer_ptr = Type *;

    template <typename Type>
    using array_observer_ptr = Type *;

    template <typename Type>
    using not_null_ptr = Type *;
}

#define df_unused_variable(variable) (void)(variable)
#define df_unused_parameter(parameter) (void)(parameter)
#define has_complexity(complexity)

#endif
