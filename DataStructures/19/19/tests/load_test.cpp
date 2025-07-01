#include <iostream>
#include <windows.h>
#include <chrono>
#include "../19.h"

class SystemMonitor {
public:
    void printMemoryStats() {
        MEMORYSTATUSEX memInfo;
        memInfo.dwLength = sizeof(memInfo);

        if (GlobalMemoryStatusEx(&memInfo)) {
            DWORDLONG totalPhysMem = memInfo.ullTotalPhys;
            DWORDLONG availPhysMem = memInfo.ullAvailPhys;

            std::cout << "Total Physical Memory: " << totalPhysMem / (1024 * 1024) << " MB" << std::endl;
            std::cout << "Available Physical Memory: " << availPhysMem / (1024 * 1024) << " MB" << std::endl;
            std::cout << "Memory Load: " << memInfo.dwMemoryLoad << " %" << std::endl;
        }
        else {
            std::cerr << "Error retrieving memory status." << std::endl;
        }
    }

    void printCPUUsage() {
        FILETIME idleTime1, kernelTime1, userTime1;
        FILETIME idleTime2, kernelTime2, userTime2;

        if (!GetSystemTimes(&idleTime1, &kernelTime1, &userTime1)) {
            std::cerr << "Error getting first system times." << std::endl;
            return;
        }

        Sleep(100);

        if (!GetSystemTimes(&idleTime2, &kernelTime2, &userTime2)) {
            std::cerr << "Error getting second system times." << std::endl;
            return;
        }

        ULONGLONG idle1 = fileTimeToULL(idleTime1);
        ULONGLONG kernel1 = fileTimeToULL(kernelTime1);
        ULONGLONG user1 = fileTimeToULL(userTime1);

        ULONGLONG idle2 = fileTimeToULL(idleTime2);
        ULONGLONG kernel2 = fileTimeToULL(kernelTime2);
        ULONGLONG user2 = fileTimeToULL(userTime2);

        ULONGLONG sys1 = kernel1 + user1;
        ULONGLONG sys2 = kernel2 + user2;

        ULONGLONG sysDiff = sys2 - sys1;
        ULONGLONG idleDiff = idle2 - idle1;

        double cpuUsage = 0.0;
        if (sysDiff != 0)
            cpuUsage = (1.0 - ((double)idleDiff / sysDiff)) * 100.0;

        std::cout << "CPU Usage: " << cpuUsage << " %" << std::endl;
    }

private:
    ULONGLONG fileTimeToULL(const FILETIME& ft) {
        return ((ULONGLONG)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
    }
};

void performanceTestCCS() {
    const int max_size = 10000;
    const int step_size = 1000;

    SystemMonitor monitor;

    for (int size = 1000; size <= max_size; size += step_size) {
        std::cout << "\nTest with matrix size: " << size << "x" << size << std::endl;

        int nz = size * size; // Количество ненулевых элементов (условное, можно оптимизировать)
        double* a = new double[nz]();
        int* li = new int[nz]();
        int* lj = new int[size + 1]();

        for (int i = 0; i < nz; ++i) {
            a[i] = rand() % 100;
            li[i] = rand() % size;
        }

        lj[0] = 0;
        for (int i = 1; i <= size; ++i) {
            lj[i] = lj[i - 1] + size;
        }

        std::cout << "System stats before test:" << std::endl;
        monitor.printMemoryStats();
        monitor.printCPUUsage();

        auto start = std::chrono::high_resolution_clock::now();

        CCSMatrix matrix;
        matrix.fillManual(size, size, nz, a, li, lj);

        matrix.sortColumnsBySum();

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        std::cout << "Execution time: " << duration.count() << " seconds" << std::endl;

        std::cout << "System stats after test:" << std::endl;
        monitor.printMemoryStats();
        monitor.printCPUUsage();

        delete[] a;
        delete[] li;
        delete[] lj;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    SetConsoleOutputCP(CP_UTF8);

    performanceTestCCS();
    return 0;
}
