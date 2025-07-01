#include <iostream>
#include <windows.h>
#include <chrono>
#include "../SparseMatrix.h"
#include <unordered_set>


class SystemMonitor {                              // Класс для мониторинга системы (память, CPU)
public:
    void printMemoryStats() {
        MEMORYSTATUSEX memInfo;
        memInfo.dwLength = sizeof(memInfo);
        if (GlobalMemoryStatusEx(&memInfo)) {
            std::cout << "Memory Load: " << memInfo.dwMemoryLoad << "%\n";
            std::cout << "Available Physical Memory: " << memInfo.ullAvailPhys / (1024 * 1024) << " MB\n";  // Доступная физическая память
        }
    }

    void printCPUUsage() {                         // Метод для вывода статистики по использованию CPU
        FILETIME idle1, kernel1, user1, idle2, kernel2, user2;
        GetSystemTimes(&idle1, &kernel1, &user1);  // Получаем начальное время системы
        Sleep(100);                                // Засыпаем на 100 мс
        GetSystemTimes(&idle2, &kernel2, &user2);  // Получаем конечное время системы

        ULONGLONG sys1 = toULL(kernel1) + toULL(user1);  // Считаем суммарное время использования процессора на начальный момент
        ULONGLONG sys2 = toULL(kernel2) + toULL(user2);  // Считаем суммарное время использования процессора на конечный момент
        ULONGLONG idleDiff = toULL(idle2) - toULL(idle1);// Разница во времени простоя
        ULONGLONG sysDiff = sys2 - sys1;                 // Разница во времени активности процессора

        if (sysDiff != 0) {
            double cpu = (1.0 - (double)idleDiff / sysDiff) * 100.0;  // Рассчитываем загрузку процессора
            std::cout << "CPU Usage: " << cpu << " %\n";              // Выводим загрузку процессора
        }
    }

private:
    ULONGLONG toULL(FILETIME t) {                                 // Преобразование FILETIME в ULONGLONG для подсчета времени
        return ((ULONGLONG)t.dwHighDateTime << 32) | t.dwLowDateTime;
    }
};

void performanceTestSparse() {
    const int max_size = 1000;  // Максимальный размер матрицы для тестирования
    const int step = 100;       // Шаг увеличения размера матрицы

    SystemMonitor monitor;      // Создаем объект мониторинга системы

    for (int size = 100; size <= max_size; size += step) {         // Запускаем тесты для различных размеров матриц
        int total = size * size;// Общее количество элементов в матрице
        int nnz = total / 2;    // Максимальное безопасное количество ненулевых элементов

        int* A = new int[nnz];
        int* LI = new int[nnz];
        int* LJ = new int[nnz];

        std::unordered_set<long long> used; // Множество для отслеживания использованных позиций
        int count = 0;
        while (count < nnz) {
            int r = rand() % size;          // Генерация случайной строки
            int c = rand() % size;          // Генерация случайного столбца
            long long key = static_cast<long long>(r) * size + c;  // Создание уникального ключа для позиции

            if (used.count(key)) continue;  // Если позиция уже использовалась, генерируем новые координаты
            used.insert(key);               // Добавляем уникальную позицию в множество

            A[count] = rand() % 100 + 1;  // Заполнение ненулевого элемента случайным значением
            LI[count] = r;                // Заполнение индекса строки
            LJ[count] = c;                // Заполнение индекса столбца
            ++count;
        }

        std::cout << "\nTest: " << size << "x" << size << ", nnz = " << nnz << std::endl;  // Выводим текущие параметры теста

        std::cout << "[Before test]\n";  // Статистика до теста
        monitor.printMemoryStats();      // Печать статистики памяти
        monitor.printCPUUsage();         // Печать статистики CPU

        auto start = std::chrono::high_resolution_clock::now();  // Засекаем время начала теста

        SparseMatrix mat;                            // Создаем объект SparseMatrix
        mat.fillManual(size, size, nnz, A, LI, LJ);  // Заполняем матрицу
        mat.rotate90();                              // Поворачиваем матрицу на 90 градусов

        auto end = std::chrono::high_resolution_clock::now();   // Засекаем время конца теста
        std::chrono::duration<double> time = end - start;       // Расчет времени выполнения теста

        std::cout << "Time: " << time.count() << " seconds\n";  // Выводим время выполнения

        std::cout << "[After test]\n";  // Статистика после теста
        monitor.printMemoryStats();     // Печать статистики памяти
        monitor.printCPUUsage();        // Печать статистики CPU

        delete[] A;
        delete[] LI;
        delete[] LJ;
    }

    std::cout << "\nLoad test completed.\n";  // Сообщение об окончании теста
}

int main() {
    setlocale(LC_ALL, "Russian");  // Устанавливаем локаль для русских символов
    SetConsoleOutputCP(CP_UTF8);   // Устанавливаем кодировку UTF-8 для вывода в консоль

    performanceTestSparse();       // Запускаем тестирование производительности

    return 0;
}
