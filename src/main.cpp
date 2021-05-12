#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>

#include "..\headers\ProfileMatrix.h"

int main()
{
    std::cout << "Work in progress ฅ^•ﻌ•^ฅ" << std::endl;
    const int SZ = 5;
    std::vector<std::vector<double>> m(SZ, std::vector<double>(SZ, 0));
    for (int i = 0; i < SZ; i++)
    {
        for (int j = 0; j < SZ; j++)
        {
            if (std::abs(i - j) < 2)
                m[i][j] = (i + 1) * 10 + (j + 1);
        }
    }
    ProfileMatrix pm(m);
    for (int i = 0; i < SZ; i++)
    {
        for (int j = 0; j < SZ; j++)
        {
            int val = pm.get(i, j);
            std::cout << std::setw(2) << val << ' ';
        }
        std::cout << std::endl;
    }
    return 0;
}