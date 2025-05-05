#include <cassert>
#include "../SSSMatrix.h"

void testEmptyMatrix() {
    SSSMatrix matrix;
    assert(matrix.sumElements() == 0 && "Сумма пустой матрицы должна быть 0");
}

void testDiagonalOnly() {
    SSSMatrix matrix;
    int ad[] = { 1, 2, 3 };    // Только диагональ
    int li[] = { 0, 0, 0, 0 }; // Нет элементов вне диагонали
    matrix.fillManual(3, 3, ad, nullptr, nullptr, nullptr, li);
    assert(matrix.sumElements() == 6 && "Сумма диагонали: 1+2+3=6");
}

void testWithOffDiagonal() {
    SSSMatrix matrix;
    int ad[] = { 1, 2, 3 };    // Диагональ
    int au[] = { 4 };          // Элемент выше диагонали (например, [0][2])
    int al[] = { 5 };          // Элемент ниже диагонали (например, [2][0])
    int lj[] = { 2 };          // Столбец для AU (индекс 2)
    int li[] = { 0, 1, 1, 1 }; // Индексы начала строк
    matrix.fillManual(3, 3, ad, au, al, lj, li);
    assert(matrix.sumElements() == 15 && "Сумма: 1+2+3 + 4 + 5 = 15");
}

void testZeroElements() {
    SSSMatrix matrix;
    int ad[] = { 0, 0, 0 };    // Нулевая диагональ
    int li[] = { 0, 0, 0, 0 };
    matrix.fillManual(3, 3, ad, nullptr, nullptr, nullptr, li);
    assert(matrix.sumElements() == 0 && "Сумма нулевой матрицы должна быть 0");
}

void testSingleElement() {
    SSSMatrix matrix;
    int ad[] = { 42 };         // Матрица 1x1
    int li[] = { 0, 0 };
    matrix.fillManual(1, 1, ad, nullptr, nullptr, nullptr, li);
    assert(matrix.sumElements() == 42 && "Сумма матрицы 1x1: 42");
}

int main() {
    testEmptyMatrix();
    testDiagonalOnly();
    testWithOffDiagonal();
    testZeroElements();
    testSingleElement();
    return 0;
}