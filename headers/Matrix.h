#pragma once

/*
 * Abstract class representing two-dimensional matrix
 */
class Matrix
{
    using value_t = double;
    using id_t = std::size_t;
public:
    /*
     * Returns value from "row" row and "col" column.
     */
    virtual value_t get(id_t row, id_t col) const = 0;

    /* 
     * Sets value "val" to "row" row and "col" column.
     */
    virtual void set(id_t row, id_t col, value_t val) = 0;
};