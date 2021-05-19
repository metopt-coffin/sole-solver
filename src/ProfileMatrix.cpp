#include "ProfileMatrix.h"

#include <algorithm>
#include <cassert>
#include <cmath>

ProfileMatrix::ProfileMatrix(const std::vector<value_vec>& matrix)
{
    assert(!matrix.empty() && "Matrix must contain at least single row");
    //assert(std::all_of(matrix.begin(), matrix.end(), [&matrix](const auto & row) { row.size() == matrix.size(); })
    //    && "Matrix must be square");

    id_t dim = matrix.size();
    diag.resize(dim);
    for (id_t i = 0; i < dim; i++)
    {
        diag[i] = matrix[i][i];
    }
    prof.resize(dim + 1);
    prof[0] = prof[1] = 1;
    for (id_t i = 1; i < dim; i++)
    {
        id_t pos = 0;
        for (; pos < i && matrix[i][pos] == 0; pos++);
        prof[i + 1] = prof[i] + i - pos;
        for (int j = pos; j < i; j++)
        {
            a_low.push_back(matrix[i][j]);
            a_up.push_back(matrix[j][i]);
        }
    }
}

ProfileMatrix::ProfileMatrix(const QuadMatrix& qm)
    : ProfileMatrix(qm.get_matrix())
{}

ProfileMatrix::ProfileMatrix(
    value_vec diag,
    value_vec low,
    value_vec up,
    id_vec prof)
    : diag(std::move(diag))
    , a_low(std::move(low))
    , a_up(std::move(up))
    , prof(std::move(prof))
{}

auto ProfileMatrix::get(id_t row, id_t col) const -> value_t /*override*/
{
    auto res = get_ptr(row, col);
    return res ? *res : 0;
}

void ProfileMatrix::set(id_t row, id_t col, value_t val) /*override*/
{
    auto changed = get_ptr(row, col);
    if (changed) {
        *changed = val;
    }
}

auto ProfileMatrix::get_ptr(id_t row, id_t col) const -> const value_t *
{
    assert(row >= 0 && col >= 0
        && row < diag.size() && col < diag.size()
        && "Row and column indexes must be in bounds to get");

    if (row == col)
    {
        return &diag[row];
    }

    bool is_lower = col < row;
    if (!is_lower)
    {
        std::swap(col, row);
    }

    id_t prof_val = prof[row + 1] - prof[row];
    if (prof_val < row - col)
    {
        return nullptr;
    }
    id_t pos = (prof[row] - 1) + prof_val - (row - col);
    return is_lower ? &a_low[pos] : &a_up[pos];
}

auto ProfileMatrix::get_ptr(id_t row, id_t col) -> value_t*
{
    return const_cast<value_t*>(const_cast<const ProfileMatrix*>(this)->get_ptr(row, col));
}

template<class T>
void print_vector(std::ostream& os, std::vector<T> vec)
{
    for (std::size_t i = 0; i < vec.size(); i++)
    {
        os << vec[i] << ' ';
    }
}
std::ostream& operator<<(std::ostream& os, const ProfileMatrix& pm)
{
    print_vector(os, pm.diag);
    os << '\n';
    print_vector(os, pm.a_low);
    os << '\n';
    print_vector(os, pm.a_up);
    os << '\n';
    print_vector(os, pm.prof);
    os << '\n';
    return os;
}
