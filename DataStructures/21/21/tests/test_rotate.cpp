#include "../SparseMatrix.h"
#include <cassert>

void testRotate90() {
    SparseMatrix matrix;
    int A[] = { 1, 2, 3 };
    int LI[] = { 0, 1, 1 };
    int LJ[] = { 0, 1, 2 };
    matrix.fillManual(2, 3, 3, A, LI, LJ);

    matrix.rotate90();

    assert(matrix.getRows() == 3);
    assert(matrix.getCols() == 2);
}

void testRotateEmpty() {
    SparseMatrix matrix;
    matrix.rotate90();
    assert(matrix.getRows() == 0);
    assert(matrix.getCols() == 0);
}

int main() {
    testRotate90();
    testRotateEmpty();
    return 0;
}
