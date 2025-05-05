#include <cassert>
#include "../SSSMatrix.h"

int main() {
    SSSMatrix matrix;
    matrix.inputFromFileAndPackageMatrix("test_matrix.txt");

    const int* li = matrix.getLI();       // Проверяем LI и LJ
    const int* lj = matrix.getLJ();
    assert(li[0] == 0);
    assert(li[1] == 1);
    assert(lj[0] == 2);

    return 0;
}