#pragma once

class Matrix
{
    using value_t = double;
    using id_t = std::size_t;
public:
    virtual value_t get(id_t row, id_t column) const = 0;
};