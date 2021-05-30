#include "GaussSolver.h"
#include "LUMatrixViews.h"
#include "ProfileMatrix.h"


/*static*/ auto GaussSolver::solve(Matrix && a, std::vector<value_t> && b) -> std::vector<value_t>
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

    for (id_t i = b.size() - 1; i != 0; --i) {
        for (id_t j = 0; j < i; ++j) {
            b[i] -= b[j] * l.get(i, j);
        }
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

    for (id_t i = 0; i < b.size(); ++i) {
        for (id_t j = i + 1; j < b.size(); ++j) {
            b[i] -= b[j] * u.get(i, j) / u.get(j, j);
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
