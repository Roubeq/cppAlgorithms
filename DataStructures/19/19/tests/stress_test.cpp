#include <iostream>
#include <chrono>
#include <cstdlib>
#include "../19.h"

// ������-�������� �������
void stressTest() {
    const int max_size = 10000;  // ������������ ������ �������
    const int step_size = 1000;  // ��� ���������� ������� �������
    const int num_tests = 10;    // ���������� ������

    std::cout << "Starting stress test..." << std::endl;

    // �������� ������-���� � ������� ��������� �������
    for (int size = 1000; size <= max_size; size += step_size) {
        std::cout << "Test with matrix size: " << size << "x" << size << std::endl;

        // ��������� ��������� ������ ��� �������
        double* a = new double[size * size];        // ������ ��� ���������
        int* li = new int[size * size];             // ������ ��� �������� �����
        int* lj = new int[size + 1];                // ������ ��� �������� ��������

        // ��������� ������� ���������� ����������
        for (int i = 0; i < size * size; ++i) {
            a[i] = rand() % 100; // ��������� �������� ��� ��������� �������
            li[i] = rand() % size; // ��������� ������� �����
        }

        // ���������� LJ �������� (������� ������ ��������)
        lj[0] = 0;
        for (int i = 1; i <= size; ++i) {
            lj[i] = lj[i - 1] + size; // ��� ��������, ��������� LJ �����������
        }

        // �������� ����� ����������
        auto start = std::chrono::high_resolution_clock::now();

        std::cout << "Starting operation with matrix size " << size << "x" << size << "..." << std::endl;

        // ������� ������ CCSMatrix � ��������� ��� �������
        CCSMatrix matrix;
        matrix.fillManual(size, size, size * size, a, li, lj);

        matrix.sortColumnsBySum();

        // ��������� ��������� �������
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        std::cout << "Execution time for matrix size " << size << "x" << size << ": "
            << duration.count() << " seconds" << std::endl;

        // ������� ������
        delete[] a;
        delete[] li;
        delete[] lj;

        std::cout << "Test for matrix " << size << "x" << size << " completed." << std::endl;
        std::cout << "---------------------------------------------" << std::endl;
    }

    std::cout << "Stress test completed." << std::endl;
}

// ������� �������
int main() {
    stressTest();
    return 0;
}