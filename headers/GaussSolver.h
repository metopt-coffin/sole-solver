#pragma once

#include "Matrix.h"

#include <vector>

class GaussSolver
{
public:
    using value_t = double;
    using id_t = std::size_t;

    struct Result
    {
        std::vector<value_t> answer;
        id_t actions;
    };

    /*
     * Solve a system of linear equations using Gauss with LU-decomposition method
     */
    static Result solve_lu(Matrix && a, std::vector<value_t> && b);
    /*
     * Solve a system of linear equations using Gauss with pivot element choice
     */
    static Result solve(Matrix && a, std::vector<value_t> && b);

};
