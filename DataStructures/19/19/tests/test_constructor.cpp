#include "../19.h"
#include <cassert>

void testDefaultConstructor() {
    CCSMatrix matrix;
    assert(matrix.getLines() == 0);
    assert(matrix.getColumns() == 0);
    assert(matrix.getNonNullElements() == 0);
    assert(matrix.getA() == nullptr);
    assert(matrix.getLI() == nullptr);
    assert(matrix.getLJ() == nullptr);
}

void testDestructor() {
    CCSMatrix* matrix = new CCSMatrix();
    delete matrix; // Не должно быть утечек памяти
}

int main() {
    testDefaultConstructor();
    testDestructor();
    return 0;
}