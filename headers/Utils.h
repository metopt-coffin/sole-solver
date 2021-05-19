#pragma once
#include <algorithm>
#include <random>
#include <vector>

#include "ProfileMatrix.h"
#include "QuadMatrix.h"

/*
 * Struct, that has only static methods-generators
 */
struct Generator
{
    /*
     * Generates a multidiagonal matrix with size of "dim"
     * and position of non-zero elements closer than "width" to the main diagonal
     */
    static QuadMatrix generate_quad_matrix(std::size_t dim, std::size_t width)
    {
        std::vector<std::vector<double>> m(dim, std::vector<double>(dim, 0.));
        for (int j = 0; j < dim; j++)
        {
            for (int i = 0; i < dim; i++)
            {
                if (j <= i && i - j < (int)width)
                {
                    m[i][j] = (rand_gen() % 100000 - 50000.) / (rand_gen() % 100000 + 1.);
                    m[j][i] = (rand_gen() % 100000 - 50000.) / (rand_gen() % 100000 + 1.);
                }
            }
        }

        return QuadMatrix(std::move(m));
    }

    /*
     * Generates a multidiagonal matrix in a profile format with size of "dim"
     * and position of non-zero elements closer than "width" to the main diagonal
     */
    static ProfileMatrix generate_profile_matrix(const std::size_t dim, const std::size_t width)
    {
        std::vector<double> diag(dim);
        for (std::size_t i = 0; i < dim; i++)
        { diag[i] = (rand_gen() % 100000 - 50000.) / (rand_gen() % 100000 + 1.); }

        std::vector<std::size_t> profile(dim + 1);
        if (dim != 0)
        { profile[0] = profile[1] = 1; }
        for (std::size_t i = 2; i <= dim; i++)
        {
            profile[i] = profile[i - 1] + rand_gen() % (std::min(width - 1, i - 1) + 1);
        }

        std::vector<double> a_low(profile.back() - 1);
        std::vector<double> a_up(profile.back() - 1);
        for (std::size_t i = 0; i < a_low.size(); i++)
        {
            a_low[i] = (rand_gen() % 100000 - 50000.) / (rand_gen() % 100000 + 1.);
            a_up[i] = (rand_gen() % 100000 - 50000.) / (rand_gen() % 100000 + 1.);
        }

        return ProfileMatrix(std::move(diag), std::move(a_low), std::move(a_up), std::move(profile));
    }

    static std::mt19937 rand_gen;
};

std::mt19937 Generator::rand_gen = std::mt19937(std::random_device()());