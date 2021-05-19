#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>

#include "ProfileMatrix.h"
#include "QuadMatrix.h"
#include "Utils.h"

int main()
{
    std::cout << "Work in progress ฅ^•ﻌ•^ฅ" << std::endl;
    const int SZ = 5;
    Generator::create_profile_matrix("test1", "prof.txt", SZ, 3);
    Generator::create_quad_matrix("test2", "quad.txt", SZ, 3);
    QuadMatrix qm = Generator::read_quad_matrix("test2", "quad.txt");
    ProfileMatrix pm = Generator::read_profile_matrix("test1", "prof.txt");
    //std::cout << std::setprecision(2);
    for (std::size_t i = 0; i < SZ; i++)
    {
        for (std::size_t j = 0; j < SZ; j++)
        {
            std::cout << std::setw(10) << qm.get(i, j) << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    for (std::size_t i = 0; i < SZ; i++)
    {
        for (std::size_t j = 0; j < SZ; j++)
        {
            std::cout << std::setw(10) << pm.get(i, j) << ' ';
        }
        std::cout << std::endl;
    }
    return 0;
}