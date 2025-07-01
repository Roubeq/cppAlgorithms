#include "../SparseMatrix.h"
#include <cassert>

void testDefaultConstructor() {
    SparseMatrix matrix;
    assert(matrix.readAndPack("nonexistent.txt") == false); // Проверка инициализации
}

void testDestructor() {
    auto* matrix = new SparseMatrix();
    delete matrix;                                          // Проверка отсутствия утечек
}

int main() {
    testDefaultConstructor();
    testDestructor();
    return 0;
}