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
        std::uniform_real_distribution<double> dist(-limit, limit);
        for (int j = 0; j < dim; j++)
        {
            for (int i = j; i < std::min(j + width, dim); i++)
            {
                m[i][j] = dist(rand_gen);
                m[j][i] = dist(rand_gen);
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
        std::uniform_real_distribution<double> dist(-limit, limit);

        std::vector<double> diag(dim);
        for (std::size_t i = 0; i < dim; i++)
        { diag[i] = dist(rand_gen); }

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
            a_low[i] = dist(rand_gen);
            a_up[i] = dist(rand_gen);
        }

        return ProfileMatrix(std::move(diag), std::move(a_low), std::move(a_up), std::move(profile));
    }
    
    /*
     * Generates a quadratic matrix and writes it down to file
     */
    static void create_quad_matrix(const std::string& dir, const std::string& filename, std::size_t dim, std::size_t width = UINT_MAX)
    {
        fs::path p = ".";
        p /= dir;
        fs::create_directory(p);
        std::ofstream os(p / filename);
        os << generate_quad_matrix(dim, width);
    }

    /*
     * Reads a qudratic matrix from file
     */
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

    /*
     * Generates a profile matrix and writes it down to file
     */
    static void create_profile_matrix(const std::string& dir, const std::string& filename, std::size_t dim, std::size_t width = UINT_MAX)
    {
        fs::path p = ".";
        p /= dir;
        fs::create_directory(p);
        std::ofstream os(p / filename);
        os << std::setw(10) << generate_profile_matrix(dim, width);
    }

    /*
     * Reads a profile matrix from file
     */
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

    /*
     * Reads a vector from file
     */
    static std::vector<double> read_vector(const std::string& dir, const std::string& filename)
    {
        fs::path p = ".";
        p /= dir;
        std::ifstream is(p / filename);

        std::vector<double> read;
        read_vec(is, read);
        return read;
    }

    /*
     * Generates vector of values with given length
     */
    static std::vector<double> generate_vector(std::size_t dim)
    {
        std::uniform_real_distribution<double> dist(-limit, limit);
        std::vector<double> answer(dim);
        for (double& elem : answer)
        {
            elem = dist(rand_gen);
        }
        return answer;
    }

    /*
     * Create a test (matrix, answer vector, right part vector) and writes to given directory with given prefix
     */
    static void create_quad_test(const std::string& dir, const std::string& testname, const std::size_t& dim, const std::size_t& width = UINT_MAX)
    {
        create_test(dir, testname, std::move(generate_quad_matrix(dim, width)), std::move(generate_vector(dim)));
    }
    static void create_profile_test(const std::string& dir, const std::string& testname, const std::size_t& dim, const std::size_t& width = UINT_MAX)
    {
        create_test(dir, testname, std::move(generate_profile_matrix(dim, width)), std::move(generate_vector(dim)));
    }

    static inline std::mt19937 rand_gen{std::random_device{}()};
    static inline double limit{ 100. };
private:
    template <class T>
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

    template <class T>
    static void print_vec(std::ostream& os, const std::vector<T>& vec)
    {
        for (const T& elem : vec)
        {
            os << elem << ' ';
        }
    }

    template <class T>
    static void create_test(const std::string& dir, const std::string& testname, T&& matrix, std::vector<double> answer)
    {
        fs::path p = ".";
        p /= dir;
        fs::create_directory(p);

        std::string matrix_filename = testname + "_matrix.txt";
        std::string answer_filename = testname + "_answer.txt";
        std::string right_part_filename = testname + "_right.txt";
        std::vector<double> right_part = matrix * answer;

        std::ofstream os_matrix(p / matrix_filename);
        std::ofstream os_answer(p / answer_filename);
        std::ofstream os_right(p / right_part_filename);
        os_matrix << matrix;
        print_vec(os_answer, answer);
        print_vec(os_right, right_part);
    }
};
