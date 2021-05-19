#include "QuadMatrix.h"

#include <cassert>

QuadMatrix::QuadMatrix(std::vector<value_vec> other_matrix)
    : matrix(std::move(other_matrix))
{}

QuadMatrix::QuadMatrix(const id_t& dim1, const id_t& dim2)
    : matrix(dim1, value_vec(dim2))
{}

QuadMatrix::QuadMatrix(const QuadMatrix& other)
    : matrix(other.matrix)
{}
QuadMatrix::QuadMatrix(QuadMatrix&& other) noexcept
    : matrix(std::move(other.matrix))
{}
QuadMatrix& QuadMatrix::operator=(const QuadMatrix& other)
{
    if (&other == this)
    {
        return *this;
    }

    matrix = other.matrix;
    return *this;
}
QuadMatrix& QuadMatrix::operator=(QuadMatrix&& other) noexcept
{
    if (&other == this)
    {
        return *this;
    }

    matrix = std::move(other.matrix);
    return *this;
}

auto QuadMatrix::get(id_t row, id_t col) const -> value_t /*override*/
{
    assert(row >= 0 && col >= 0 && row < matrix.size() && (!matrix.size() || col < matrix.front().size())
        && "Indexes out of bounds in QuadMatrix get");
    return matrix[row][col];
}

void QuadMatrix::set(id_t row, id_t col, value_t val) /*override*/
{
    assert(row >= 0 && col >= 0 && row < matrix.size() && (!matrix.size() || col < matrix.front().size())
        && "Indexes out of bounds in QuadMatrix set");
    matrix[row][col] = val;
}