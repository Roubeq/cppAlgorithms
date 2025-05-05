#include <iostream>
#include <chrono>
#include <cstdlib>
#include "../SSSMatrix.h"

void stressTest() {              // Функция стресс-тестирования
    const int max_size = 10000;  // Максимальный размер матрицы
    const int step_size = 1000;  // Шаг увеличения размера матрицы
    const int num_tests = 10;    // Количество тестов (можно настроить)

    std::cout << "Starting stress test..." << std::endl;

    for (int size = 1000; size <= max_size; size += step_size) {
        std::cout << "Test with matrix size: " << size << "x" << size << std::endl;

        int* ad = new int[size];
        int* au = new int[size * size];
        int* al = new int[size * size];
        int* lj = new int[size * size];
        int* li = new int[size + 1];

        for (int i = 0; i < size; ++i) {                // Заполнение массивов случайными значениями
            ad[i] = rand() % 100;
        }

        for (int i = 0; i < size * size; ++i) {         // Заполнение AU, AL, LJ, LI случайными значениями (для тестирования)
            au[i] = rand() % 100;
            al[i] = rand() % 100;
            lj[i] = rand() % size;
        }

        for (int i = 0; i <= size; ++i) {
            li[i] = i * size / (size + 1);              // Просто генерируем индекс начала строки
        }

        auto start = std::chrono::high_resolution_clock::now();

        std::cout << "Starting operation with matrix size " << size << "x" << size << "..." << std::endl;

        SSSMatrix matrix;
        matrix.fillManual(size, size, ad, au, al, lj, li);      // Создание объекта SSSMatrix и заполнение его данными

        matrix.sumElements();                                   // Выполнение некоторых операций (например, суммирование элементов)

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