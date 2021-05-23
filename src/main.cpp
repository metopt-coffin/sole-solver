#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>

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
    auto decomposition = ProfileMatrix::lu_decompose(std::move(qm));
    std::cout << "decomposition:\n";
    print_matrix(decomposition);
    std::cout << '\n';
    LMatrixView l_view{decomposition};
    UMatrixView u_view{decomposition};

    std::cout << "L:\n";
    print_matrix(l_view);
    std::cout << '\n';
    print_matrix_wolfram(l_view);

    std::cout << "\nU:\n";
    print_matrix(u_view);
    std::cout << '\n';
    print_matrix_wolfram(u_view);
    std::cout << '\n';
    return 0;
}