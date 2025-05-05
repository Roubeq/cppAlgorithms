#include <cassert>
#include "../SSSMatrix.h"

void testEmptyMatrix() {
    SSSMatrix matrix;
    assert(matrix.sumElements() == 0 && "����� ������ ������� ������ ���� 0");
}

void testDiagonalOnly() {
    SSSMatrix matrix;
    int ad[] = { 1, 2, 3 };    // ������ ���������
    int li[] = { 0, 0, 0, 0 }; // ��� ��������� ��� ���������
    matrix.fillManual(3, 3, ad, nullptr, nullptr, nullptr, li);
    assert(matrix.sumElements() == 6 && "����� ���������: 1+2+3=6");
}

void testWithOffDiagonal() {
    SSSMatrix matrix;
    int ad[] = { 1, 2, 3 };    // ���������
    int au[] = { 4 };          // ������� ���� ��������� (��������, [0][2])
    int al[] = { 5 };          // ������� ���� ��������� (��������, [2][0])
    int lj[] = { 2 };          // ������� ��� AU (������ 2)
    int li[] = { 0, 1, 1, 1 }; // ������� ������ �����
    matrix.fillManual(3, 3, ad, au, al, lj, li);
    assert(matrix.sumElements() == 15 && "�����: 1+2+3 + 4 + 5 = 15");
}

void testZeroElements() {
    SSSMatrix matrix;
    int ad[] = { 0, 0, 0 };    // ������� ���������
    int li[] = { 0, 0, 0, 0 };
    matrix.fillManual(3, 3, ad, nullptr, nullptr, nullptr, li);
    assert(matrix.sumElements() == 0 && "����� ������� ������� ������ ���� 0");
}

void testSingleElement() {
    SSSMatrix matrix;
    int ad[] = { 42 };         // ������� 1x1
    int li[] = { 0, 0 };
    matrix.fillManual(1, 1, ad, nullptr, nullptr, nullptr, li);
    assert(matrix.sumElements() == 42 && "����� ������� 1x1: 42");
}

int main() {
    testEmptyMatrix();
    testDiagonalOnly();
    testWithOffDiagonal();
    testZeroElements();
    testSingleElement();
    return 0;
}