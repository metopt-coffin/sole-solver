#pragma once
#include <vector>
#include "Matrix.h"

class ProfileMatrix : public Matrix
{
    using value_t = double;
    using value_vec = std::vector<value_t>;
    using id_t = std::size_t;
    using id_vec = std::vector<id_t>;
public:
    ProfileMatrix(const std::vector<value_vec>& matrix);

    ProfileMatrix(
        const value_vec diag,
        const value_vec low,
        const value_vec up,
        const id_vec prof);

    ProfileMatrix(const ProfileMatrix& other);

    ProfileMatrix(ProfileMatrix&& other) noexcept;

    ProfileMatrix& operator=(const ProfileMatrix& other);

    ProfileMatrix& operator=(ProfileMatrix&& other) noexcept;

    virtual value_t get(id_t row, id_t col) const /*override*/;
private:
    value_vec diag;
    value_vec a_low;
    value_vec a_up;
    id_vec prof;
};