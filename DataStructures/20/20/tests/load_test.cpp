#include <iostream>
#include <windows.h>
#include <chrono>
#include "../SSSMatrix.h"

class SystemMonitor {
public:
    void printMemoryStats() {
        MEMORYSTATUSEX memInfo;
        memInfo.dwLength = sizeof(memInfo);

        if (GlobalMemoryStatusEx(&memInfo)) {
            DWORDLONG totalPhysMem = memInfo.ullTotalPhys;
            DWORDLONG availPhysMem = memInfo.ullAvailPhys;

            std::cout << "Total Physical Memory: " << totalPhysMem / (1024 * 1024) << " MB" << std::endl;      // Печать общего объема физической памяти
            std::cout << "Available Physical Memory: " << availPhysMem / (1024 * 1024) << " MB" << std::endl;  // Печать доступного объема физической памяти
            std::cout << "Memory Load: " << memInfo.dwMemoryLoad << " %" << std::endl;                         // Печать загрузки памяти в процентах
        }
        else {
            std::cerr << "Error retrieving memory status." << std::endl;  // Ошибка получения статуса памяти
        }
    }

    void printCPUUsage() {
        FILETIME idleTime1, kernelTime1, userTime1;
        FILETIME idleTime2, kernelTime2, userTime2;

        if (!GetSystemTimes(&idleTime1, &kernelTime1, &userTime1)) {      // Получение системного времени для первого измерения
            std::cerr << "Error getting first system times." << std::endl;
            return;
        }

        Sleep(100);             // Небольшая пауза для замера

        if (!GetSystemTimes(&idleTime2, &kernelTime2, &userTime2)) {      // Получение системного времени для второго измерения
            std::cerr << "Error getting second system times." << std::endl;
            return;
        }

        ULONGLONG idle1 = fileTimeToULL(idleTime1);      // Преобразование времени простоя в формат ULONGLONG
        ULONGLONG kernel1 = fileTimeToULL(kernelTime1);  // Преобразование времени работы ядра в формат ULONGLONG
        ULONGLONG user1 = fileTimeToULL(userTime1);      // Преобразование времени пользовательского режима в формат ULONGLONG

        ULONGLONG idle2 = fileTimeToULL(idleTime2);      // Преобразование времени простоя во втором измерении
        ULONGLONG kernel2 = fileTimeToULL(kernelTime2);  // Преобразование времени работы ядра во втором измерении
        ULONGLONG user2 = fileTimeToULL(userTime2);      // Преобразование времени пользовательского режима во втором измерении

        ULONGLONG sys1 = kernel1 + user1;                // Общее системное время для первого измерения
        ULONGLONG sys2 = kernel2 + user2;                // Общее системное время для второго измерения

        ULONGLONG sysDiff = sys2 - sys1;                 // Разница в системном времени
        ULONGLONG idleDiff = idle2 - idle1;              // Разница в времени простоя

        double cpuUsage = 0.0;
        if (sysDiff != 0)
            cpuUsage = (1.0 - ((double)idleDiff / sysDiff)) * 100.0;  // Вычисление загрузки CPU

        std::cout << "CPU Usage: " << cpuUsage << " %" << std::endl;  // Печать использования CPU
    }

private:
    ULONGLONG fileTimeToULL(const FILETIME& ft) {  // Функция преобразования FILETIME в ULONGLONG
        return ((ULONGLONG)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
    }
};

void performanceTest() {
    const int max_size = 10000;  // Максимальный размер матрицы
    const int step_size = 1000;  // Шаг увеличения размера матрицы

    SystemMonitor monitor;       // Создание объекта для мониторинга системы

    for (int size = 1000; size <= max_size; size += step_size) {  // Цикл для тестирования с разными размерами матрицы
        std::cout << "\nTest with matrix size: " << size << "x" << size << std::endl;

        int* ad = new int[size]();
        int* au = new int[size * size]();
        int* al = new int[size * size]();
        int* lj = new int[size * size]();
        int* li = new int[size + 1]();

        for (int i = 0; i < size; ++i) ad[i] = rand() % 100;  // Заполнение элементов
        for (int i = 0; i < size * size; ++i) {
            au[i] = rand() % 100;
            al[i] = rand() % 100;
            lj[i] = rand() % size;
        }
        for (int i = 0; i <= size; ++i) li[i] = i * size / (size + 1);

        std::cout << "System stats before test:" << std::endl;
        monitor.printMemoryStats();                            // Печать статистики по памяти до теста
        monitor.printCPUUsage();                               // Печать статистики по использованию CPU до теста

        auto start = std::chrono::high_resolution_clock::now();// Засечка времени начала теста

        SSSMatrix matrix;
        matrix.fillManual(size, size, ad, au, al, lj, li);     // Заполнение матрицы вручную
        int sum = matrix.sumElements();                        // Вычисление суммы элементов матрицы

        auto end = std::chrono::high_resolution_clock::now();  // Засечка времени окончания теста
        std::chrono::duration<double> duration = end - start;  // Вычисление времени выполнения теста

        std::cout << "Execution time: " << duration.count() << " seconds" << std::endl;
        std::cout << "Sum: " << sum << std::endl;

        std::cout << "System stats after test:" << std::endl;
        monitor.printMemoryStats();
        monitor.printCPUUsage();

        delete[] ad;
        delete[] au;
        delete[] al;
        delete[] lj;
        delete[] li;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    SetConsoleOutputCP(CP_UTF8);

    performanceTest();
    return 0;
}
