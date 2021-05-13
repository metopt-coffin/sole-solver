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
    for (int i = 0; i < SZ; i++)
    {
        for (int j = 0; j < SZ; j++)
        {
            double val = pm.get(i, j);
            
            std::cout << std::setw(5) << val << ' ';
        }
        std::cout << std::endl;
    }
    return 0;
}