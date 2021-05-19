#pragma once
#include <algorithm>
#include <climits>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <random>
#include <string>
#include <sstream>
#include <vector>

#include "ProfileMatrix.h"
#include "QuadMatrix.h"
namespace fs = std::filesystem;
/*
 * Struct, that has only static methods-generators
 */
struct Generator
{
    /*
     * Generates a multidiagonal matrix with size of "dim"
     * and position of non-zero elements closer than "width" to the main diagonal
     */
    static QuadMatrix generate_quad_matrix(std::size_t dim, std::size_t width = UINT_MAX)
    {
        std::vector<std::vector<double>> m(dim, std::vector<double>(dim, 0.));
        for (int j = 0; j < dim; j++)
        {
            for (int i = 0; i < dim; i++)
            {
                if (j <= i && i - j < width)
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
    static ProfileMatrix generate_profile_matrix(const std::size_t dim, const std::size_t width = UINT_MAX)
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
    
    static void create_quad_matrix(const std::string& dir, const std::string& filename, std::size_t dim, std::size_t width = UINT_MAX)
    {
        fs::path p = ".";
        p /= dir;
        fs::create_directory(p);
        std::ofstream os(p / filename);
        os << generate_quad_matrix(dim, width);
    }

    static QuadMatrix read_quad_matrix(const std::string& dir, const std::string& filename)
    {
        fs::path p = ".";
        p /= dir;
        std::ifstream is(p / filename);
        std::string line;
        std::vector<std::vector<double>> m;
        while (std::getline(is, line))
        {
            m.emplace_back(std::vector<double>());
            std::istringstream strs(line);
            double val;
            while (strs >> val)
            {
                m.back().push_back(val);
            }
        }

        return m;
    }

    static void create_profile_matrix(const std::string& dir, const std::string& filename, std::size_t dim, std::size_t width = UINT_MAX)
    {
        fs::path p = ".";
        p /= dir;
        fs::create_directory(p);
        std::ofstream os(p / filename);
        os << std::setw(10) << generate_profile_matrix(dim, width);
    }

    static ProfileMatrix read_profile_matrix(const std::string& dir, const std::string& filename)
    {
        fs::path p = ".";
        p /= dir;
        std::ifstream is(p / filename);

        
        std::vector<double> diag;
        std::vector<double> low;
        std::vector<double> up;
        std::vector<std::size_t> prof;
        read_vec(is, diag);
        read_vec(is, low);
        read_vec(is, up);
        read_vec(is, prof);

        return ProfileMatrix(std::move(diag), std::move(low), std::move(up), std::move(prof));
    }

    static std::mt19937 rand_gen;
private:
    template<class T>
    static void read_vec(std::istream& is, std::vector<T>& vec)
    {
        std::string line;
        std::getline(is, line);
        std::istringstream strs(line);
        T val;
        while (strs >> val)
        {
            vec.push_back(val);
        }
    }
};

std::mt19937 Generator::rand_gen = std::mt19937(std::random_device()());