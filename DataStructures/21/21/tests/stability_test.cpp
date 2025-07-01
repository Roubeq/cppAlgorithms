#include <iostream>
#include <unordered_set>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include "../SparseMatrix.h"

bool arraysEqual(int* a1, int* a2, int size) {
    for (int i = 0; i < size; ++i) {
        if (a1[i] != a2[i]) return false;
    }
    return true;
}

void stabilityTestSparse() {          // ������� ��� ������������ ������������ SparseMatrix
    const int size = 100;             // ������ �������
    const int nnz = size * size / 4;  // 25% ��������� ���������
    const int iterations = 1000;      // ���������� ���������� �����

    std::cout << "=== SparseMatrix Stability Test ===\n";  // ������ �����
    std::cout << "Size: " << size << "x" << size << ", NNZ = " << nnz << ", Iterations: " << iterations << "\n";  // ��������� �����

    // �������� ������ ��� �������
    int* A_original = new int[nnz];
    int* LI_original = new int[nnz];
    int* LJ_original = new int[nnz];

    std::unordered_set<long long> used;                         // ��������� ��� �������� ������������ ��������
    int count = 0;
    while (count < nnz) {
        int r = rand() % size;  // ��������� ��������� ������
        int c = rand() % size;  // ��������� ���������� �������
        long long key = static_cast<long long>(r) * size + c;  // �������� ����������� �����

        if (used.count(key)) continue;                         // ���� ������� ��� ������������, ���������� ����� ����������

        used.insert(key);        // ��������� ���������� ������� � ���������
        A_original[count] = rand() % 100 + 1;  // ��������� ��������� ������� ��������� ���������
        LI_original[count] = r;  // ��������� ���������� ������
        LJ_original[count] = c;  // ��������� ���������� �������
        ++count;
    }

    for (int i = 1; i <= iterations; ++i) {
        SparseMatrix mat;
        mat.fillManual(size, size, nnz, A_original, LI_original, LJ_original);  // ��������� �������

        for (int j = 0; j < 4; ++j) {                           // ��������� 4 �������� �� 90 ��������
            mat.rotate90();
        }

        int* A_tested = new int[nnz];                           // ��������� ������ �� ���������� �������
        int* LI_tested = new int[nnz];
        int* LJ_tested = new int[nnz];

        mat.extractData(A_tested, LI_tested, LJ_tested);        // ��������� ������ � ������� ������ extractData

        if (!arraysEqual(A_original, A_tested, nnz) ||          // ���������, ��� ������ �� ���������� ����� ���������
            !arraysEqual(LI_original, LI_tested, nnz) ||
            !arraysEqual(LJ_original, LJ_tested, nnz)) {

            std::cerr << "Error: Matrix changed after 4 rotations on iteration " << i << "!\n";  // ������, ���� ������� ����������
            delete[] A_tested;
            delete[] LI_tested;
            delete[] LJ_tested;
            assert(false);                                      // ��������� ����, ���� ������� ����������
        }

        delete[] A_tested;
        delete[] LI_tested;
        delete[] LJ_tested;

        if (i % 100 == 0) {
            std::cout << "Iteration " << i << " completed successfully.\n";  // �������� ���������� ��������
        }
    }

    std::cout << "Stability test completed successfully.\n";    // ���������� �����

    delete[] A_original;
    delete[] LI_original;
    delete[] LJ_original;
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));  // ������������� ���������� ��������� �����
    stabilityTestSparse();
    return 0;
}