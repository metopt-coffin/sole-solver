#pragma once

#include "Matrix.h"

#include <vector>

class GaussSolver
{
public:
    using value_t = double;
    using id_t = std::size_t;

    static std::vector<value_t> solve(Matrix && a, std::vector<value_t> && b);

};
