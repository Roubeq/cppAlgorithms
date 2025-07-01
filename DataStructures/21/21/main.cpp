#include "SparseMatrix.h"

int main() {
    setlocale(LC_ALL, "Russian");
    SparseMatrix matrix;
    matrix.rotate90();
    matrix.unpackAndPrint();
    matrix.printArrays();
    return 0;
}