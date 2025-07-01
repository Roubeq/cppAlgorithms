#include <iostream>
#include <cstdlib>
#include <chrono>
#include <unordered_set>
#include <windows.h>
#include "../SparseMatrix.h"  // ���� � ������ SparseMatrix.h

void stressTest() {
    const int max_size = 3000;  // ������������ ������ ������� 
    const int step_size = 1000;  // ��� ���������� ������� �������

    std::cout << "Starting stress test..." << std::endl;

    // �������� ������-���� � ������� ��������� �������
    for (int size = 1000; size <= max_size; size += step_size) {
        int total = size * size;
        int max_possible_nnz = total / 2;  // ����������� ���������� ���������� ��������� ���������

        std::cout << "Test with matrix size: " << size << "x" << size << std::endl;

        // ��������� ��������� ������ ��� �������
        int nnz = max_possible_nnz;  // ���������� ��������� ��������� (����������� ���������)

        int* A = new int[nnz];        // ������ ��� ��������� ���������
        int* LI = new int[nnz];       // ������ ��� �������� �����
        int* LJ = new int[nnz];       // ������ ��� �������� ��������

        // ���������� ��������� ��� ���������� �������
        std::unordered_set<long long> used;
        int count = 0;
        while (count < nnz) {
            int r = rand() % size;
            int c = rand() % size;
            long long key = static_cast<long long>(r) * size + c;

            if (used.count(key)) continue; // ���� ������� ��� ������������, ����������
            used.insert(key);

            A[count] = rand() % 100 + 1;  // ��������� ��������� ��������
            LI[count] = r;
            LJ[count] = c;
            ++count;
        }

        // �������� ����� ���������� ��������
        auto start = std::chrono::high_resolution_clock::now();

        std::cout << "Starting operation with matrix size " << size << "x" << size << "..." << std::endl;

        // ������� ������ SparseMatrix � ��������� ��� �������
        SparseMatrix mat;
        mat.fillManual(size, size, nnz, A, LI, LJ);

        mat.rotate90();  // ������ �������� (������� ������� �� 90 ��������)

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        std::cout << "Execution time for matrix size " << size << "x" << size << ": "
            << duration.count() << " seconds" << std::endl;

        // ������� ������
        delete[] A;
        delete[] LI;
        delete[] LJ;

        std::cout << "Test for matrix " << size << "x" << size << " completed." << std::endl;
        std::cout << "---------------------------------------------" << std::endl;
    }

    std::cout << "Stress test completed." << std::endl;
}

int main() {
    setlocale(LC_ALL, "Russian");
    SetConsoleOutputCP(CP_UTF8);

    stressTest();

    return 0;
}
