#include <iostream>
#include <chrono>
#include <cstdlib>
#include "../19.h"

// Стресс-тестовая функция
void stressTest() {
    const int max_size = 10000;  // Максимальный размер матрицы
    const int step_size = 1000;  // Шаг увеличения размера матрицы
    const int num_tests = 10;    // Количество тестов

    std::cout << "Starting stress test..." << std::endl;

    // Стартуем стресс-тест с разными размерами матрицы
    for (int size = 1000; size <= max_size; size += step_size) {
        std::cout << "Test with matrix size: " << size << "x" << size << std::endl;

        // Генерация случайных данных для матрицы
        double* a = new double[size * size];        // Массив для элементов
        int* li = new int[size * size];             // Массив для индексов строк
        int* lj = new int[size + 1];                // Массив для индексов столбцов

        // Заполняем массивы случайными значениями
        for (int i = 0; i < size * size; ++i) {
            a[i] = rand() % 100; // Случайные значения для элементов матрицы
            li[i] = rand() % size; // Случайные индексы строк
        }

        // Заполнение LJ индексов (индексы начала столбцов)
        lj[0] = 0;
        for (int i = 1; i <= size; ++i) {
            lj[i] = lj[i - 1] + size; // Для простоты, заполняем LJ увеличением
        }

        // Измеряем время выполнения
        auto start = std::chrono::high_resolution_clock::now();

        std::cout << "Starting operation with matrix size " << size << "x" << size << "..." << std::endl;

        // Создаем объект CCSMatrix и заполняем его данными
        CCSMatrix matrix;
        matrix.fillManual(size, size, size * size, a, li, lj);

        matrix.sortColumnsBySum();

        // Завершаем измерение времени
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        std::cout << "Execution time for matrix size " << size << "x" << size << ": "
            << duration.count() << " seconds" << std::endl;

        // Очистка памяти
        delete[] a;
        delete[] li;
        delete[] lj;

        std::cout << "Test for matrix " << size << "x" << size << " completed." << std::endl;
        std::cout << "---------------------------------------------" << std::endl;
    }

    std::cout << "Stress test completed." << std::endl;
}

// Главная функция
int main() {
    stressTest();
    return 0;
}