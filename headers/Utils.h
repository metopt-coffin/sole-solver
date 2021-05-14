#pragma once
#include <random>
#include <vector>

#include "ProfileMatrix.h"

/*
 * Struct, that has only static methods-generators
 */
struct Generator
{
    /*
     * Generates a diagonal matrix in profile format with size of "dim"
     * and position of non-zero elements closer than "width" to the main diagonal
     * TODO NOTE: THIS IS A DRAFT THAT USES O(n^2) MEMORY AND TIME. 
     * MORE APPROPRIATE IMPLEMENTATION IS REQUIRED
     */
    static ProfileMatrix generate_profile_matrix(std::size_t dim, std::size_t width)
    {
        std::vector<std::vector<double>> m(dim, std::vector<double>(dim, 0.));
        for (int j = 0; j < dim; j++)
        {
            for (int i = 0; i < dim; i++)
            {
                if (j <= i)
                {
                    if (i - j < width)
                    {
                        m[i][j] = (rand_gen() % 100000 - 50000.) / (rand_gen() % 100000 + 1.);
                    }
                }
                else
                {
                    m[i][j] = m[j][i];
                }
            }
        }

        return ProfileMatrix(m);
    }

    static std::mt19937 rand_gen;
};

std::mt19937 Generator::rand_gen = std::mt19937(std::random_device()());