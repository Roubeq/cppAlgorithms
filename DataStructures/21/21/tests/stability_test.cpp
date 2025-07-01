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

void stabilityTestSparse() {          // Функция для тестирования стабильности SparseMatrix
    const int size = 100;             // Размер матрицы
    const int nnz = size * size / 4;  // 25% ненулевых элементов
    const int iterations = 1000;      // Количество повторений теста

    std::cout << "=== SparseMatrix Stability Test ===\n";  // Начало теста
    std::cout << "Size: " << size << "x" << size << ", NNZ = " << nnz << ", Iterations: " << iterations << "\n";  // Параметры теста

    // Исходные данные для матрицы
    int* A_original = new int[nnz];
    int* LI_original = new int[nnz];
    int* LJ_original = new int[nnz];

    std::unordered_set<long long> used;                         // Множество для проверки уникальности индексов
    int count = 0;
    while (count < nnz) {
        int r = rand() % size;  // Генерация случайной строки
        int c = rand() % size;  // Генерация случайного столбца
        long long key = static_cast<long long>(r) * size + c;  // Создание уникального ключа

        if (used.count(key)) continue;                         // Если позиция уже используется, генерируем новые координаты

        used.insert(key);        // Добавляем уникальную позицию в множество
        A_original[count] = rand() % 100 + 1;  // Заполняем ненулевой элемент случайным значением
        LI_original[count] = r;  // Заполняем индексацию строки
        LJ_original[count] = c;  // Заполняем индексацию столбца
        ++count;
    }

    for (int i = 1; i <= iterations; ++i) {
        SparseMatrix mat;
        mat.fillManual(size, size, nnz, A_original, LI_original, LJ_original);  // Заполняем матрицу

        for (int j = 0; j < 4; ++j) {                           // Применяем 4 поворота на 90 градусов
            mat.rotate90();
        }

        int* A_tested = new int[nnz];                           // Извлекаем данные из полученной матрицы
        int* LI_tested = new int[nnz];
        int* LJ_tested = new int[nnz];

        mat.extractData(A_tested, LI_tested, LJ_tested);        // Извлекаем данные с помощью метода extractData

        if (!arraysEqual(A_original, A_tested, nnz) ||          // Проверяем, что данные не изменились после поворотов
            !arraysEqual(LI_original, LI_tested, nnz) ||
            !arraysEqual(LJ_original, LJ_tested, nnz)) {

            std::cerr << "Error: Matrix changed after 4 rotations on iteration " << i << "!\n";  // Ошибка, если матрица изменилась
            delete[] A_tested;
            delete[] LI_tested;
            delete[] LJ_tested;
            assert(false);                                      // Прерываем тест, если матрица изменилась
        }

        delete[] A_tested;
        delete[] LI_tested;
        delete[] LJ_tested;

        if (i % 100 == 0) {
            std::cout << "Iteration " << i << " completed successfully.\n";  // Успешное завершение итерации
        }
    }

    std::cout << "Stability test completed successfully.\n";    // Завершение теста

    delete[] A_original;
    delete[] LI_original;
    delete[] LJ_original;
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));  // Инициализация генератора случайных чисел
    stabilityTestSparse();
    return 0;
}