#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

#include "Solver.h"
#include "LUMatrixViews.h"
#include "Matrix.h"
#include "ProfileMatrix.h"
#include "SparseMatrix.h"
#include "QuadMatrix.h"
#include "Utils.h"

void print_matrix(const Matrix & matrix)
{
    for (unsigned int i = 0; i < matrix.row_cnt(); ++i) {
        for (unsigned int j = 0; j < matrix.col_cnt(); ++j) {
            std::cout << std::setw(12) << matrix.get(i, j) << ' ';
        }
        std::cout << '\n';
    }
}

void print_matrix_wolfram(const Matrix & matrix)
{
    std::cout << "{";
    for (unsigned int i = 0; i < matrix.row_cnt(); ++i) {
        std::cout << "{";
        for (unsigned int j = 0; j < matrix.col_cnt(); ++j) {
            std::cout << matrix.get(i, j);
            if (j != matrix.col_cnt() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << '}';
        if (i != matrix.row_cnt() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "}";
}

void print_sole_volfram(const Matrix & a, const std::vector<double> & b)
{
    for (unsigned i = 0; i < a.row_cnt(); ++i) {
        for (unsigned j = 0; j < a.col_cnt(); ++j) {
            std::cout << (a.get(i, j) >= 0 ? "+" : "") << a.get(i, j) << " * " << static_cast<char>(j + 'a');
        }
        std::cout << " = " << b[i] << '\n';
    }
}

template <class T>
Solver::Result solve_sole(T&& a, std::vector<double>&& b)
{
    print_sole_volfram(a, b);
    return Solver::solve_gauss(std::move(a), std::move(b));
}

template <class T>
Solver::Result generate_and_solve_sole(T && a)
{
    std::mt19937 rand(std::random_device{}());
    std::uniform_real_distribution<double> dist(-10., 10.);

    std::vector<double> b(a.col_cnt());
    std::generate(b.begin(), b.end(), [&] { return dist(rand); });
    print_sole_volfram(a, b);
    return solve_sole(std::move(a), std::move(b));
}

int main()
{
    std::cout << std::setprecision(10);
    QuadMatrix qm = Generator::generate_quad_matrix(5);
    auto res = generate_and_solve_sole(std::move(qm));
    if (res.actions == Solver::Result::FAILED)
    {
        std::cout << "Method failed: division on zero element spotted" << std::endl;
    }
    else
    {
        std::cout << "Answer is: \n";
        for (double el : res.answer)
        {
            std::cout << el << " ";
        }
        std::cout << std::endl;
        if (res.actions == Solver::Result::UNTRUSTED)
        {
            std::cout << "Results cannot be trusted";
        }
        else
        {
            std::cout << "Actions: " << res.actions;
        }
        std::cout << std::endl;
    }
    return 0;
}