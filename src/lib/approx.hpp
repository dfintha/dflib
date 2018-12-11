#if !defined(DFLIB_APPROX)
#define DFLIB_APPROX

namespace df {
    inline unsigned precision = 10;

    template <typename Real>
    Real pow(Real x, unsigned n);

    template <typename Real>
    Real fact(unsigned x);

    template <typename Real>
    Real sin(Real x);

    template <typename Real>
    Real cos(Real x);

    // ------------------------------------------------------------------ //

    template <typename Real>
    Real pow(Real x, unsigned n) {
        Real result = 1.0;
        for (unsigned i = 0; i < n; ++i)
            result *= x;
        return result;
    }

    template <typename Real>
    Real fact(unsigned x) {
        Real result = 1.0;
        for (unsigned i = 0; i < x; ++i)
            result *= (x - i);
        return result;
    }

    template <typename Real>
    Real sin(Real x) {
        Real result = 0.0;
        for (unsigned i = 0; i < precision; ++i) {
            const Real a = df::pow(-1, i);
            const Real b = df::fact<Real>(2 * i + 1);
            const Real c = df::pow(x, 2 * i + 1);
            result += (a / b) * c;
        }
        return result;
    }

    template <typename Real>
    Real cos(Real x) {
        Real result = 0.0;
        for (unsigned i = 0; i < precision; ++i) {
            const Real a = df::pow(-1, i);
            const Real b = df::fact<Real>(2 * i);
            const Real c = df::pow(x, 2 * i);
            result += (a / b) * c;
        }
        return result;
    }
}

#endif
