#include "../SSSMatrix.h"
#include <cassert>

int main() {
    SSSMatrix matrix;
    assert(matrix.sumElements() == 0);              // ���� 1: ����������� �� ���������

    SSSMatrix* dynamicMatrix = new SSSMatrix();     // ���� 2: ���������� (�������� ����� valgrind)
    delete dynamicMatrix;

    return 0;
}