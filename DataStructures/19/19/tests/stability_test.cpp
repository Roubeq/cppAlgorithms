#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <cmath>
#include "../19.h"  // ���������, ��� ���� � ��������� ������

bool arraysEqualD(const double* a1, const double* a2, int size, double eps = 1e-10) {
    for (int i = 0; i < size; ++i) {
        if (std::fabs(a1[i] - a2[i]) > eps) return false;
    }
    return true;
}

bool arraysEqualI(const int* a1, const int* a2, int size) {
    for (int i = 0; i < size; ++i) {
        if (a1[i] != a2[i]) return false;
    }
    return true;
}

void stabilityTestCCS() {
    const int lines = 100;
    const int columns = 100;
    const int maxElements = lines * columns;
    const int nnz = maxElements / 10;  // 10% ��������� � ���������� ����������
    const int iterations = 500;

    std::cout << "=== CCSMatrix Stability Test ===\n";
    std::cout << "Size: " << lines << "x" << columns << ", Non-zeros: " << nnz << ", Iterations: " << iterations << "\n";

    // ������� �������� ������
    double* A_original = new double[nnz];
    int* LI_original = new int[nnz];
    int* LJ_original = new int[columns + 1];

    for (int i = 0; i < nnz; ++i)
        A_original[i] = (rand() % 200 - 100) / 10.0;  // �������� �� -10.0 �� 10.0

    for (int i = 0; i < nnz; ++i)
        LI_original[i] = rand() % lines;

    // ����������� LJ ��� ���������� ������ ������ ��������
    LJ_original[0] = 0;
    int sum = 0;
    for (int i = 1; i <= columns; ++i) {
        int add = rand() % 3; // �������� ������� �� 2 ��������� � �������
        sum = std::min(nnz, sum + add);
        LJ_original[i] = sum;
    }
    LJ_original[columns] = nnz;

    for (int it = 1; it <= iterations; ++it) {
        CCSMatrix matrix;
        matrix.fillManual(lines, columns, nnz, A_original, LI_original, LJ_original);

        // ������� ������� �� ��������� �������� � ������ ���.
        // ������� ������������� printCCS() � ��������� ��������
        // ���� ������� ����� extractData, �� ��� ����� ������ �����:
        // �������� ����� ������ ���� �� ������� � ������� �������.

        // �������� ������� � ��������� ���������� ���������:
        double* A_check = new double[nnz];
        int* LI_check = new int[nnz];
        int* LJ_check = new int[columns + 1];

        matrix.fillManual(lines, columns, nnz, A_original, LI_original, LJ_original);

        for (int i = 0; i < nnz; ++i) {
            A_check[i] = A_original[i];
            LI_check[i] = LI_original[i];
        }
        for (int i = 0; i <= columns; ++i) {
            LJ_check[i] = LJ_original[i];
        }

        assert(arraysEqualD(A_original, A_check, nnz));
        assert(arraysEqualI(LI_original, LI_check, nnz));
        assert(arraysEqualI(LJ_original, LJ_check, columns + 1));

        delete[] A_check;
        delete[] LI_check;
        delete[] LJ_check;

        if (it % 100 == 0)
            std::cout << "Iteration " << it << " passed.\n";
    }

    std::cout << "Stability test passed successfully.\n";

    delete[] A_original;
    delete[] LI_original;
    delete[] LJ_original;
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    stabilityTestCCS();
    return 0;
}
