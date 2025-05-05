#include <iostream>
#include <chrono>
#include <cstdlib>
#include "../SSSMatrix.h"

void stressTest() {              // ������� ������-������������
    const int max_size = 10000;  // ������������ ������ �������
    const int step_size = 1000;  // ��� ���������� ������� �������
    const int num_tests = 10;    // ���������� ������ (����� ���������)

    std::cout << "Starting stress test..." << std::endl;

    for (int size = 1000; size <= max_size; size += step_size) {
        std::cout << "Test with matrix size: " << size << "x" << size << std::endl;

        int* ad = new int[size];
        int* au = new int[size * size];
        int* al = new int[size * size];
        int* lj = new int[size * size];
        int* li = new int[size + 1];

        for (int i = 0; i < size; ++i) {                // ���������� �������� ���������� ����������
            ad[i] = rand() % 100;
        }

        for (int i = 0; i < size * size; ++i) {         // ���������� AU, AL, LJ, LI ���������� ���������� (��� ������������)
            au[i] = rand() % 100;
            al[i] = rand() % 100;
            lj[i] = rand() % size;
        }

        for (int i = 0; i <= size; ++i) {
            li[i] = i * size / (size + 1);              // ������ ���������� ������ ������ ������
        }

        auto start = std::chrono::high_resolution_clock::now();

        std::cout << "Starting operation with matrix size " << size << "x" << size << "..." << std::endl;

        SSSMatrix matrix;
        matrix.fillManual(size, size, ad, au, al, lj, li);      // �������� ������� SSSMatrix � ���������� ��� �������

        matrix.sumElements();                                   // ���������� ��������� �������� (��������, ������������ ���������)

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        std::cout << "Execution time for matrix size " << size << "x" << size << ": "
            << duration.count() << " seconds" << std::endl;

        delete[] ad;
        delete[] au;
        delete[] al;
        delete[] lj;
        delete[] li;

        std::cout << "Test for matrix " << size << "x" << size << " completed." << std::endl;
        std::cout << "---------------------------------------------" << std::endl;
    }

    std::cout << "Stress test completed." << std::endl;
}

int main() {
    stressTest();
    return 0;
}