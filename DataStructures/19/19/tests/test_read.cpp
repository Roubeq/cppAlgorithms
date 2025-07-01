#include "../19.h"
#include <cassert>
#include <stdexcept>
#include <string>
#include <iostream>

void testReadValidMatrix() {
    // �������:
    // 1 0 0
    // 0 2 0
    // 0 0 3

    double A[] = { 1.0, 2.0, 3.0 };
    int LI[] = { 0, 1, 2 };         // ������, ��� ����������� ��������
    int LJ[] = { 0, 1, 2, 3 };      // ������� ��������

    CCSMatrix matrix;
    matrix.fillManual(3, 3, 3, A, LI, LJ);

    assert(matrix.getLines() == 3);
    assert(matrix.getColumns() == 3);
    assert(matrix.getNonNullElements() == 3);

    const double* data = matrix.getA();
    assert(data[0] == 1.0);
    assert(data[1] == 2.0);
    assert(data[2] == 3.0);

    std::cout << "Test Read Valid Matrix: PASSED" << std::endl;
}

void testReadInvalidFile() {
    CCSMatrix matrix;
    bool exceptionThrown = false;
    try {
        matrix.readAndPack("nonexistent.txt"); // �� ��� ��������� ��������� �� �������������� �����
    }
    catch (const std::exception&) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);

    std::cout << "Test Read Invalid File: PASSED" << std::endl;
}

void testReadDenseMatrixManual() {
    // �������: 1 1
    //          1 1
    // ��� �� �����������, �� �� ���������� � �������, ��������� readAndPack

    double A[] = { 1.0, 1.0, 1.0, 1.0 };
    int LI[] = { 0, 1, 0, 1 };
    int LJ[] = { 0, 2, 4 };

    CCSMatrix matrix;
    matrix.fillManual(2, 2, 4, A, LI, LJ);

    assert(matrix.getNonNullElements() == 4);

    std::cout << "Test Read Dense Matrix Manual: PASSED" << std::endl;
}

void testManualEmptyMatrix() {
    int LJ[] = { 0, 0, 0 };         // ��� �������, �� ������ �� ��� �������� 0 ���������

    CCSMatrix matrix;
    bool exceptionThrown = false;
    try {
        matrix.fillManual(2, 2, 0, 0, 0, LJ); // 2x2 �������, 0 ���������
    }
    catch (const std::exception&) {
        exceptionThrown = true;
    }

    assert(exceptionThrown && "Expected exception on manually filled empty matrix");

    std::cout << "Test Manual Empty Matrix: PASSED" << std::endl;
}

int main() {
    testReadValidMatrix();
    testReadInvalidFile();
    testReadDenseMatrixManual();
    testManualEmptyMatrix();

    return 0;
}
