#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <random>
#include <vector>

#include "GaussSolver.h"
#include "LUMatrixViews.h"
#include "Matrix.h"
#include "ProfileMatrix.h"
#include "QuadMatrix.h"
#include "Utils.h"

void print_matrix(const Matrix & matrix)
{
    for (uint i = 0; i < matrix.row_cnt(); ++i) {
        for (uint j = 0; j < matrix.col_cnt(); ++j) {
            std::cout << std::setw(12) << matrix.get(i, j) << ' ';
        }
        std::cout << '\n';
    }
}

void print_matrix_wolfram(const Matrix & matrix)
{
    std::cout << "{";
    for (uint i = 0; i < matrix.row_cnt(); ++i) {
        std::cout << "{";
        for (uint j = 0; j < matrix.col_cnt(); ++j) {
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

void generate_and_solve_sole(Matrix && a)
{
    std::mt19937 rand(std::random_device{}());
    std::uniform_real_distribution<double> dist(-100., 100.);

    std::vector<double> b(a.col_cnt());
    std::generate(b.begin(), b.end(), [&] { return dist(rand); });

    print_sole_volfram(a, b);
    std::cout << "\nresult:\n";
    for (double el : GaussSolver::solve(std::move(a), std::move(b))) {
        std::cout << el << " ";
    }
    std::cout << '\n';
}

int main()
{
    std::cout << std::setprecision(5);
    std::cout << "Work in progress ฅ^•ﻌ•^ฅ" << std::endl;
    const int SZ = 4;
    Generator::create_profile_matrix("test2", "prof.txt", SZ, 3);
    Generator::create_quad_matrix("test1", "quad.txt", SZ);
    QuadMatrix qm = Generator::read_quad_matrix("test1", "quad.txt");
    ProfileMatrix pm = Generator::read_profile_matrix("test2", "prof.txt");
    //std::cout << std::setprecision(2);
    // for (std::size_t i = 0; i < SZ; i++)
    // {
    //     for (std::size_t j = 0; j < SZ; j++)
    //     {
    //         std::cout << std::setw(10) << qm.get(i, j) << ' ';
    //     }
    //     std::cout << std::endl;
    // }
    // std::cout << std::endl;
    // for (std::size_t i = 0; i < SZ; i++)
    // {
    //     for (std::size_t j = 0; j < SZ; j++)
    //     {
    //         std::cout << std::setw(10) << pm.get(i, j) << ' ';
    //     }
    //     std::cout << std::endl;
    // }
    print_matrix(qm);
    std::cout << '\n';
    generate_and_solve_sole(std::move(qm));
    return 0;
}