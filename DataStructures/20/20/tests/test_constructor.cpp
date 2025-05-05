#include "../SSSMatrix.h"
#include <cassert>

int main() {
    SSSMatrix matrix;
    assert(matrix.sumElements() == 0);              // Тест 1: Конструктор по умолчанию

    SSSMatrix* dynamicMatrix = new SSSMatrix();     // Тест 2: Деструктор (проверка через valgrind)
    delete dynamicMatrix;

    return 0;
}