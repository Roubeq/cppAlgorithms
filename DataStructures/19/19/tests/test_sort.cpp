#include "../19.h"
#include <cassert>
#include <fstream>
#include <cstdio>

void createTestFile(const char* filename, const char* content) {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error(std::string("�� ������� ������� ����: ") + filename);
    }
    file << content;
}

void testSortColumns() {
    CCSMatrix matrix;

    // �������:
    // 1 0 3
    // 0 4 0
    // 2 0 5

    // CCS:
    // A: 1 2 4 3 5
    // LI: 0 2 1 0 2
    // LJ: 0 2 3 5

    double A[] = { 1, 2, 4, 3, 5 };
    int LI[] = { 0, 2, 1, 0, 2 };
    int LJ[] = { 0, 2, 3, 5 };

    matrix.fillManual(3, 3, 5, A, LI, LJ);
    matrix.sortColumnsBySum();

    matrix.printCCS();              // ���������� �����
    matrix.reconstructAndPrint();   // ��������� �������� �������

    // ����� � assert'��� ��������� �������, ��������:
    const double* sortedA = matrix.getA();
    const int* sortedLJ = matrix.getLJ();

    assert(sortedA[sortedLJ[0]] == 1.0);  // ������ ��������������� ������� � 1, 2
    assert(sortedA[sortedLJ[1]] == 4.0);  // ��������� � 4
    assert(sortedA[sortedLJ[2]] == 3.0);  // � ������� 3, 5
    assert(sortedA[sortedLJ[2] + 1] == 5.0);
}


void testSortEmptyMatrix() {
    CCSMatrix matrix;
    bool exceptionThrown = false;
    try {
        matrix.sortColumnsBySum();
    }
    catch (const std::exception& e) {
        exceptionThrown = true;
        assert(std::string(e.what()) == "������� �� ���������������� ��� ����������");
    }
    assert(exceptionThrown);
}

int main() {
    testSortColumns();
    testSortEmptyMatrix();
    return 0;
}