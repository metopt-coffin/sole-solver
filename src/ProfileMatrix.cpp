#include "ProfileMatrix.h"

#include <algorithm>
#include <cassert>
#include <cmath>

ProfileMatrix::ProfileMatrix(const std::vector<value_vec>& matrix)
{
    id_t dim = std::max(matrix.size(), matrix.size() > 0 ? matrix[0].size() : -1);
    diag.resize(dim);
    for (id_t i = 0; i < dim; i++)
    {
        diag[i] = matrix[i][i];
    }
    prof.resize(dim + 1);
    prof[0] = prof[1] = 1;
    for (id_t i = 1; i < dim; i++)
    {
        int pos = 0;
        for (; pos < i && matrix[i][pos] == 0; pos++);
        prof[i + 1] = prof[i] + i - pos;
        for (int j = pos; j < i; j++)
        {
            a_low.push_back(matrix[i][j]);
            a_up.push_back(matrix[j][i]);
        }
    }
}

ProfileMatrix::ProfileMatrix(
    const value_vec diag,
    const value_vec low,
    const value_vec up,
    const id_vec prof)
    : diag(std::move(diag))
    , a_low(std::move(low))
    , a_up(std::move(up))
    , prof(std::move(prof))
{}

ProfileMatrix::ProfileMatrix(const ProfileMatrix& other)
    : diag(other.diag)
    , a_low(other.a_low)
    , a_up(other.a_up)
    , prof(other.prof)
{}

ProfileMatrix::ProfileMatrix(ProfileMatrix&& other) noexcept
    : diag(std::move(other.diag))
    , a_low(std::move(other.a_low))
    , a_up(std::move(other.a_up))
    , prof(std::move(other.prof))
{}

ProfileMatrix& ProfileMatrix::operator=(const ProfileMatrix& other)
{
    if (&other == this)
    {
        return *this;
    }

    diag = other.diag;
    a_low = other.a_low;
    a_up = other.a_up;
    prof = other.prof;

    return *this;
}

ProfileMatrix& ProfileMatrix::operator=(ProfileMatrix&& other) noexcept
{
    if (&other == this)
    {
        return *this;
    }

    diag = std::move(other.diag);
    a_low = std::move(other.a_low);
    a_up = std::move(other.a_up);
    prof = std::move(other.prof);

    return *this;
}

auto ProfileMatrix::get(id_t row, id_t col) const -> value_t /*override*/
{
    assert(row >= 0 && col >= 0
        && row < diag.size() && col < diag.size()
        && "Row and column indexes must be in bounds to get");

    if (row == col)
    {
        return diag[row];
    }

    bool is_lower = col < row;
    if (!is_lower)
    {
        std::swap(col, row);
    }

    unsigned int prof_val = prof[row + 1] - prof[row];
    if (prof_val < row - col)
    {
        return 0;
    }
    id_t pos = (prof[row] - 1) + prof_val - (row - col);
    return is_lower ? a_low[pos] : a_up[pos];
}

void ProfileMatrix::set(id_t row, id_t col, value_t val)
{
    assert(row >= 0 && col >= 0
        && row < diag.size() && col < diag.size()
        && "Row and column indexes must be in bounds to set");

    if (row == col)
    {
        diag[row] = val;
        return;
    }

    bool is_lower = col < row;
    if (!is_lower)
    {
        std::swap(col, row);
    }

    unsigned int prof_val = prof[row + 1] - prof[row];
    if (prof_val < row - col)
    {
        return; // do nothing, because we cannot change our profile
    }
    id_t pos = (prof[row] - 1) + prof_val - (row - col);
    value_t& cell = is_lower ? a_low[pos] : a_up[pos];
    cell = val;
}