#include "../SparseMatrix.h"
#include <cassert>

void testDefaultConstructor() {
    SparseMatrix matrix;
    assert(matrix.readAndPack("nonexistent.txt") == false); // �������� �������������
}

void testDestructor() {
    auto* matrix = new SparseMatrix();
    delete matrix;                                          // �������� ���������� ������
}

int main() {
    testDefaultConstructor();
    testDestructor();
    return 0;
}