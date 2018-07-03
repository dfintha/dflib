#if !defined(DFLIB_MATRIX)
#define DFLIB_MATRIX

#include <array>

namespace df {
    template <unsigned Rows, unsigned Columns>
    class matrix {
    public:
        static constexpr const unsigned max_length =
            Rows > Columns ? Rows : Columns;

        using row_type = std::array<double, Columns>;
        using vector_type = std::array<double, max_length>;
        using eigens_type = std::array<vector_type, max_length>;
        using submatrix_type = matrix<Rows - 1, Columns - 1>;

        double determinant() const;
        eigens_type eigenvalues() const;
        matrix transpose() const;
        matrix inverse() const;
        matrix gaussian_elimination() const;
        submatrix_type submatrix(unsigned row, unsigned column) const;
        row_type& operator[](unsigned index);
        const row_type& operator[](unsigned index) const;

        static matrix identity();
    };

    template <unsigned Rows, unsigned Columns>
    df::matrix<Rows, Columns> operator+(const df::matrix<Rows, Columns>& lhs,
                                        const df::matrix<Rows, Columns>& rhs);

    template <unsigned Rows, unsigned Columns>
    df::matrix<Rows, Columns> operator-(const df::matrix<Rows, Columns>& lhs,
                                        const df::matrix<Rows, Columns>& rhs);

    template <unsigned Rows, unsigned Columns>
    df::matrix<Rows, Columns> operator*(const df::matrix<Rows, Columns>& lhs,
                                        const df::matrix<Rows, Columns>& rhs);

    template <unsigned Rows, unsigned Columns>
    df::matrix<Rows, Columns> operator*(const df::matrix<Rows, Columns>& lhs,
                                        double rhs);

    template <unsigned Rows, unsigned Columns>
    df::matrix<Rows, Columns> operator*(double lhs,
                                        const df::matrix<Rows, Columns>& rhs);

    template <unsigned Rows, unsigned Columns>
    df::matrix<Rows, Columns> operator^(const df::matrix<Rows, Columns>& lhs,
                                        unsigned rhs);
}

#endif
