#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>

#include "ProfileMatrix.h"
#include "Utils.h"

int main()
{
    std::cout << "Work in progress ฅ^•ﻌ•^ฅ" << std::endl;
    const int SZ = 5;
    ProfileMatrix pm = Generator::generate_profile_matrix(SZ, 2);
    std::cout << std::setprecision(2);
    for (std::size_t i = 0; i < SZ; i++)
    {
        for (std::size_t j = 0; j < SZ; j++)
        {
            std::cout << std::setw(5) << pm.get(i, j) << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    for (std::size_t i = 0; i < SZ; i++)
    {
        for (std::size_t j = 0; j < SZ; j++)
        {
            pm.set(i, j, i * 10 + j);

            std::cout << std::setw(5) << pm.get(i, j) << ' ';
        }
        std::cout << std::endl;
    }
    return 0;
}