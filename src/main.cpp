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
void solve_sole(T&& a, std::vector<double>&& b)
{
    print_sole_volfram(a, b);
    T a_copy = std::move(a);
    std::vector<double> b_copy = b;

    std::cout << "\nLU result:\n";
    auto res = GaussSolver::solve_lu(T(a_copy), std::move(b_copy));
    for (double el : res.answer)
    {
        std::cout << el << " ";
    }
    std::cout << std::endl;
    std::cout << "Actions: " << res.actions;
    std::cout << std::endl;

    std::cout << "\nChoice result:\n";
    res = GaussSolver::solve(std::move(a_copy), std::move(b));
    for (double el : res.answer)
    {
        std::cout << el << " ";
    }
    std::cout << std::endl;
    std::cout << "Actions: " << res.actions;
    std::cout << '\n';
}

template <class T>
void generate_and_solve_sole(T && a)
{
    std::mt19937 rand(std::random_device{}());
    std::uniform_real_distribution<double> dist(-100., 100.);

    std::vector<double> b(a.col_cnt());
    std::generate(b.begin(), b.end(), [&] { return dist(rand); });
    solve_sole(std::move(a), std::move(b));
}

int main()
{
    std::cout << std::setprecision(5);
    std::cout << "Work in progress ฅ^•ﻌ•^ฅ" << std::endl;
    const int SZ = 4;
    const std::string dir = "quad_test";
    const std::string pref = "1";
    Generator::create_quad_test(dir, pref, SZ);
    QuadMatrix qm = Generator::read_quad_matrix(dir, pref + "_matrix.txt");
    std::vector<double> answer = Generator::read_vector(dir, pref + "_answer.txt");
    std::vector<double> right = Generator::read_vector(dir, pref + "_right.txt");
    print_matrix(qm);
    std::cout << std::endl;
    solve_sole(std::move(qm), std::move(right));
    std::cout << std::endl;
    std::cout << "Ideal answer:\n";
    for (const auto& elem : answer)
    {
        std::cout << elem << ' ';
    }
    return 0;
}