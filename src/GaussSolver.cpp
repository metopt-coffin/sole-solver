#include "GaussSolver.h"
#include "LUMatrixViews.h"
#include "ProfileMatrix.h"

#include <cmath>
#include <numeric>

/*static*/ auto GaussSolver::solve(Matrix&& a, std::vector<value_t>&& b) -> std::vector<value_t>
{
    /*
     * We don't know what kind of matrix we received.
     * Therefore, we can't swap rows in-place and have to remember all permutations we made.
     */
    std::vector<id_t> permutations(b.size());
    std::iota(permutations.begin(), permutations.end(), 0);         // permutations[i] = i;

    // forward
    for (int i = 0; i < b.size(); i++)
    {
        /*
         * Starting from here, 
         *  "virtual" - indexes in matrix with swapped rows
         *  "physical" - indexes in real matrix.
         * Usually, we don't have to map column indexes as they remain the same. 
         */
        id_t i_row = permutations[i];                               // get physical index of current row
        value_t pivot = a.get(i_row, i);                            // and current max pivot element
        id_t pivot_row = i;                                         // remember the virtual index of max pivot row

        /*
         * Find the max pivot element.
         * Look up in the next rows, because previous rows are already processed
         */
        for (int j = i + 1; j < b.size(); j++)
        {
            id_t new_row = permutations[j];                         // get physical index of row we look at now
            value_t new_val = a.get(new_row, i);                    // get next value from physical matrix
            if (new_val > pivot)                                    // relaxate maximum
            {
                pivot = new_val;
                pivot_row = j;
            }
        }
        std::swap(permutations[i], permutations[pivot_row]);        // "swap" rows in virtual matrix
        i_row = permutations[i];                                    // get physical index of current row (after swap)
        /*
         * Perform row operations and process the rest of the matrix
         */
        for (int j = i + 1; j < b.size(); j++)
        {
            id_t j_row = permutations[j];                           // get physical index of row we change now
            value_t factor = a.get(j_row, i) / a.get(i_row, i);     // get factor that will be used to change values in this row
            b[j_row] -= factor * b[i_row];                          // change the vector b
            /*
             * Process the row using formula: 
             *  a[i][j] = a[i][j] - factor * a[k][j], where k - index of row we subtract
             */
            for (int k = i + 1; k < b.size(); k++)
            {
                value_t val = a.get(j_row, k);                      // get previous value
                a.set(j_row, k, val - factor * a.get(i_row, k));    // set new value to the same place
            }
        }
    }

    
    // reverse
    /*
     * Now we have an upper triangular matrix. 
     * To get the solution we have to transfrom it to diagonal matrix, 
     *  changing the vector b accordingly
     */
    for (int i = b.size() - 1; i >= 0; i--)
    {
        id_t row = permutations[i];                                 // get physical index of current row
        for (int j = b.size() - 1; j > i; j--)
        {
            b[row] -= a.get(row, j) * b[permutations[j]];           // subtract all elements a[i][j] * x[j], where j > i;
        }                                                           //  we know answer x[j] as it is the already counted value in the vector b
        b[row] /= a.get(row, i);                                    // divide by diagonal element a[i][i];
    }
    
    /*
     * Write down the result vector in correct order
     */
    std::vector<value_t> answer(b.size());
    for (int i = 0; i < b.size(); i++)
    {
        answer[i] = b[permutations[i]];
    }
    return answer;
}

/*static*/ auto GaussSolver::solve_lu(Matrix && a, std::vector<value_t> && b) -> std::vector<value_t>
{
/*
    for i = [0; n):
       b'[i] = b[i] - sum(j = [0; i); b[j] * a[i][j] / a[j][j])
    Will go in reverse order, so we neither change values that are used later, nor copy `b` vector

    For L matrix a[j][j] is always 1 for any j
*/
    ProfileMatrix pm = ProfileMatrix::lu_decompose(std::move(a));
    LMatrixView l(pm);
    UMatrixView u(pm);

    /*for (id_t i = b.size() - 1; i != 0; --i) {
        for (id_t j = 0; j < i; ++j) {
            b[i] -= b[j] * (l.get(i, j) / l.get(j, j));
        }
    }*/

    for (int i = 0; i < b.size(); i++)
    {
        for (int j = i + 1; j < b.size(); j++)
        {
            b[j] -= b[i] * l.get(j, i) / l.get(i, i);
        }
        //b[i] /= l.get(i, i);
    }

/*
    Ax = b
    |A = LU|
    LUx = b
    |Ux = y|
    Ly = b
    After changes L = diag(1...1), y = b'
    Ux = y <=> Ux = b'

    If we also turn U into diag matrix: diag(u_11, u_22, ... , u_nn), then x[i] = b''[i] / u[i][i]
    for i = [0; n)
        b''[i] = b'[i] - sum(j = [i + 1; n); b'[j] * u[i][j] / u[j][j])
    In straigth order
*/

    /*for (id_t i = 0; i < b.size(); ++i) {
        for (id_t j = i + 1; j < b.size(); ++j) {
            b[i] -= b[j] * u.get(i, j) / u.get(j, j);
        }
    }*/

    for (int i = b.size() - 1; i >= 0; i--)
    {
        for (int j = i - 1; j >= 0; j--)
        {
            b[j] -= b[i] * u.get(j, i) / u.get(i, i);
        }
    }

/*
    As was said, x[i] = b''[i] / u[i][i]
*/
    for (id_t i = 0; i < b.size(); ++i) {
        b[i] /= u.get(i, i);
    }
    return b;
}
