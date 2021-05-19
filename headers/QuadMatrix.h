#pragma once
#include <vector>
#include "Matrix.h"

/*
 * Class, representing two-dimensional matrix contained without any optimizations
 */
class QuadMatrix : public Matrix
{
    using value_t = double;
    using value_vec = std::vector<value_t>;
    using id_t = std::size_t;
public:
    /*
     * Construct a QuadMatrix directly from a standard two-dimensional matrix.
     */
    QuadMatrix(std::vector<value_vec> matrix);

    /*
     * Construct a zero QuadMatrix with dim1 rows and dim2 columns.
     */
    QuadMatrix(id_t dim1, id_t dim2);

    /*
     * Copy and move constructors and assign operators.
     */
    QuadMatrix(const QuadMatrix& other) = default;
    QuadMatrix(QuadMatrix&& other) = default;
    QuadMatrix& operator=(const QuadMatrix& other) = default;
    QuadMatrix& operator=(QuadMatrix&& other) = default;

    /*
     *Returns value from "row" rowand "col" column.
     */
    virtual value_t get(id_t row, id_t col) const override;

    /*
     * Sets value "val" to "row" row and "col" column.
     */
    virtual void set(id_t row, id_t col, value_t val) override;
private:
    std::vector<value_vec> matrix; // two-dimensional vector, containing the matrix
};