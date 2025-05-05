#include <iostream>
#include <cstdlib>
#include <chrono>
#include <unordered_set>
#include <windows.h>
#include "../SparseMatrix.h"  // Путь к вашему SparseMatrix.h

void stressTest() {
    const int max_size = 3000;  // Максимальный размер матрицы 
    const int step_size = 1000;  // Шаг увеличения размера матрицы

    std::cout << "Starting stress test..." << std::endl;

    // Стартуем стресс-тест с разными размерами матрицы
    for (int size = 1000; size <= max_size; size += step_size) {
        int total = size * size;
        int max_possible_nnz = total / 2;  // Максимально допустимое количество ненулевых элементов

        std::cout << "Test with matrix size: " << size << "x" << size << std::endl;

        // Генерация случайных данных для матрицы
        int nnz = max_possible_nnz;  // Количество ненулевых элементов (максимально возможное)

        int* A = new int[nnz];        // Массив для ненулевых элементов
        int* LI = new int[nnz];       // Массив для индексов строк
        int* LJ = new int[nnz];       // Массив для индексов столбцов

        // Используем множество для уникальных позиций
        std::unordered_set<long long> used;
        int count = 0;
        while (count < nnz) {
            int r = rand() % size;
            int c = rand() % size;
            long long key = static_cast<long long>(r) * size + c;

            if (used.count(key)) continue; // Если позиция уже использована, пропускаем
            used.insert(key);

            A[count] = rand() % 100 + 1;  // Случайное ненулевое значение
            LI[count] = r;
            LJ[count] = c;
            ++count;
        }

        // Измеряем время выполнения операции
        auto start = std::chrono::high_resolution_clock::now();

        std::cout << "Starting operation with matrix size " << size << "x" << size << "..." << std::endl;

        // Создаем объект SparseMatrix и заполняем его данными
        SparseMatrix mat;
        mat.fillManual(size, size, nnz, A, LI, LJ);

        mat.rotate90();  // Пример операции (поворот матрицы на 90 градусов)

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        std::cout << "Execution time for matrix size " << size << "x" << size << ": "
            << duration.count() << " seconds" << std::endl;

        // Очистка памяти
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
