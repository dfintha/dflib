#if !defined(DFLIB_RESOURCE)
#define DFLIB_RESOURCE

#include <cstdio>
#include <cstdlib>
#include <utility>

namespace df {
    template <typename Type, auto Obtain, auto Release>
    class resource {
    public:
        static constexpr const bool noexcept_release =
            noexcept(Release(std::declval<Type>()));

        template <typename... Arguments>
        resource(Arguments&&... args);

        explicit resource(Type source);
        resource(const resource&) = delete;
        resource(resource&&) = delete;
        resource& operator=(const resource&) = delete;
        resource& operator=(resource&&) = delete;
        ~resource() noexcept(noexcept_release);

        template <typename... Arguments>
        void reset(Arguments&&... args);

        void reset(Type source);
        Type& get();
        const Type& get() const;
        operator Type&();
        operator const Type&() const;

    private:
        Type handle;
    };

    using c_file = resource<FILE *, fopen, fclose>;
    using c_memory = resource<void *, malloc, free>;

    // ------------------------------------------------------------------ //

    template <typename Type, auto Obtain, auto Release>
    template <typename... Arguments>
    resource<Type, Obtain, Release>::resource(Arguments&&... args) {
        handle = Obtain(std::forward<Arguments&&>(args)...);
    }

    template <typename Type, auto Obtain, auto Release>
    resource<Type, Obtain, Release>::resource(Type source) :
        handle(source)
    {  }

    template <typename Type, auto Obtain, auto Release>
    resource<Type, Obtain, Release>::~resource() noexcept(noexcept_release) {
        Release(handle);
    }

    template <typename Type, auto Obtain, auto Release>
    template <typename... Arguments>
    void resource<Type, Obtain, Release>::reset(Arguments&&... args) {
        Release(handle);
        handle = Obtain(std::forward<Arguments&&>(args)...);
    }

    template <typename Type, auto Obtain, auto Release>
    void resource<Type, Obtain, Release>::reset(Type source) {
        Release(handle);
        handle = source;
    }

    template <typename Type, auto Obtain, auto Release>
    Type& resource<Type, Obtain, Release>::get() {
        return handle;
    }

    template <typename Type, auto Obtain, auto Release>
    const Type& resource<Type, Obtain, Release>::get() const {
        return handle;
    }

    template <typename Type, auto Obtain, auto Release>
    resource<Type, Obtain, Release>::operator Type&() {
        return handle;
    }

    template <typename Type, auto Obtain, auto Release>
    resource<Type, Obtain, Release>::operator const Type&() const {
        return handle;
    }
}

#endif
