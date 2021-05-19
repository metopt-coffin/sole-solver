#pragma once
#include <vector>
#include <iostream>
#include "Matrix.h"
#include "QuadMatrix.h"

/*
 * Class, representing two-dimensional matrix contained in the profile format
 */
class ProfileMatrix : public Matrix
{
    using value_t = double;
    using value_vec = std::vector<value_t>;
    using id_t = std::size_t;
    using id_vec = std::vector<id_t>;
public:
    /*
     * Constructs a ProfileMatrix from a standard two-dimensional square matrix. 
     */
    ProfileMatrix(const std::vector<value_vec>& matrix);

    ProfileMatrix(const QuadMatrix& qm);

    /*
     * Constructs a ProfileMatrix directly from respective vectors
     * Note, that vectors are not checked to be valid vectors of profile matrix.
     */
    ProfileMatrix(
        value_vec diag,
        value_vec low,
        value_vec up,
        id_vec prof);

    /*
     * Copy and move constructors and assign operators.
     */
    ProfileMatrix(const ProfileMatrix& other) = default;
    ProfileMatrix(ProfileMatrix&& other)= default;
    ProfileMatrix& operator=(const ProfileMatrix& other) = default;
    ProfileMatrix& operator=(ProfileMatrix&& other) = default;

    /*
     * Returns value from "row" row and "col" column.
     * If the required cell is inside profile, than the corresponding value is returned.
     * Otherwise, returns 0.
     */
    virtual value_t get(id_t row, id_t col) const override;

    /*
     * Sets value "val" to "row" row and "col" column.
     * If the required cell is inside profile, than the corresponding value is set.
     * Otherwise, does nothing.
     */
    virtual void set(id_t row, id_t col, value_t val) override;

    friend std::ostream& operator<<(std::ostream& os, const ProfileMatrix& pm);
private:
    const value_t * get_ptr(id_t row, id_t col) const;
    value_t * get_ptr(id_t row, id_t col);

private:
    value_vec diag; // vector, containing diagonal elements
    value_vec a_low; // vector, containing elements from the lower part of matrix
    value_vec a_up; // vector, containing elements from the upper part of matrix
    id_vec prof; // vector, containing profile of the matrix
};