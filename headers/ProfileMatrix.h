#pragma once
#include <algorithm>
#include <cassert>
#include <cmath>
#include <vector>
#include "Matrix.h"

class ProfileMatrix : public Matrix
{
    using value_t = double;
    using id_t = std::size_t;
public:
    ProfileMatrix(const std::vector<std::vector<value_t>>& matrix);

    ProfileMatrix(const ProfileMatrix& other);

    ProfileMatrix(ProfileMatrix&& other);

    ProfileMatrix& operator=(const ProfileMatrix& other);

    ProfileMatrix& operator=(ProfileMatrix&& other);

    virtual value_t get(id_t row, id_t col) const /*override*/;
private:
    std::vector<value_t> diag;
    std::vector<value_t> a_low;
    std::vector<value_t> a_up;
    std::vector<id_t> prof;
};