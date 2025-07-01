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

            std::cout << "Total Physical Memory: " << totalPhysMem / (1024 * 1024) << " MB" << std::endl;      // ������ ������ ������ ���������� ������
            std::cout << "Available Physical Memory: " << availPhysMem / (1024 * 1024) << " MB" << std::endl;  // ������ ���������� ������ ���������� ������
            std::cout << "Memory Load: " << memInfo.dwMemoryLoad << " %" << std::endl;                         // ������ �������� ������ � ���������
        }
        else {
            std::cerr << "Error retrieving memory status." << std::endl;  // ������ ��������� ������� ������
        }
    }

    void printCPUUsage() {
        FILETIME idleTime1, kernelTime1, userTime1;
        FILETIME idleTime2, kernelTime2, userTime2;

        if (!GetSystemTimes(&idleTime1, &kernelTime1, &userTime1)) {      // ��������� ���������� ������� ��� ������� ���������
            std::cerr << "Error getting first system times." << std::endl;
            return;
        }

        Sleep(100);             // ��������� ����� ��� ������

        if (!GetSystemTimes(&idleTime2, &kernelTime2, &userTime2)) {      // ��������� ���������� ������� ��� ������� ���������
            std::cerr << "Error getting second system times." << std::endl;
            return;
        }

        ULONGLONG idle1 = fileTimeToULL(idleTime1);      // �������������� ������� ������� � ������ ULONGLONG
        ULONGLONG kernel1 = fileTimeToULL(kernelTime1);  // �������������� ������� ������ ���� � ������ ULONGLONG
        ULONGLONG user1 = fileTimeToULL(userTime1);      // �������������� ������� ����������������� ������ � ������ ULONGLONG

        ULONGLONG idle2 = fileTimeToULL(idleTime2);      // �������������� ������� ������� �� ������ ���������
        ULONGLONG kernel2 = fileTimeToULL(kernelTime2);  // �������������� ������� ������ ���� �� ������ ���������
        ULONGLONG user2 = fileTimeToULL(userTime2);      // �������������� ������� ����������������� ������ �� ������ ���������

        ULONGLONG sys1 = kernel1 + user1;                // ����� ��������� ����� ��� ������� ���������
        ULONGLONG sys2 = kernel2 + user2;                // ����� ��������� ����� ��� ������� ���������

        ULONGLONG sysDiff = sys2 - sys1;                 // ������� � ��������� �������
        ULONGLONG idleDiff = idle2 - idle1;              // ������� � ������� �������

        double cpuUsage = 0.0;
        if (sysDiff != 0)
            cpuUsage = (1.0 - ((double)idleDiff / sysDiff)) * 100.0;  // ���������� �������� CPU

        std::cout << "CPU Usage: " << cpuUsage << " %" << std::endl;  // ������ ������������� CPU
    }

private:
    ULONGLONG fileTimeToULL(const FILETIME& ft) {  // ������� �������������� FILETIME � ULONGLONG
        return ((ULONGLONG)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
    }
};

void performanceTest() {
    const int max_size = 10000;  // ������������ ������ �������
    const int step_size = 1000;  // ��� ���������� ������� �������

    SystemMonitor monitor;       // �������� ������� ��� ����������� �������

    for (int size = 1000; size <= max_size; size += step_size) {  // ���� ��� ������������ � ������� ��������� �������
        std::cout << "\nTest with matrix size: " << size << "x" << size << std::endl;

        int* ad = new int[size]();
        int* au = new int[size * size]();
        int* al = new int[size * size]();
        int* lj = new int[size * size]();
        int* li = new int[size + 1]();

        for (int i = 0; i < size; ++i) ad[i] = rand() % 100;  // ���������� ���������
        for (int i = 0; i < size * size; ++i) {
            au[i] = rand() % 100;
            al[i] = rand() % 100;
            lj[i] = rand() % size;
        }
        for (int i = 0; i <= size; ++i) li[i] = i * size / (size + 1);

        std::cout << "System stats before test:" << std::endl;
        monitor.printMemoryStats();                            // ������ ���������� �� ������ �� �����
        monitor.printCPUUsage();                               // ������ ���������� �� ������������� CPU �� �����

        auto start = std::chrono::high_resolution_clock::now();// ������� ������� ������ �����

        SSSMatrix matrix;
        matrix.fillManual(size, size, ad, au, al, lj, li);     // ���������� ������� �������
        int sum = matrix.sumElements();                        // ���������� ����� ��������� �������

        auto end = std::chrono::high_resolution_clock::now();  // ������� ������� ��������� �����
        std::chrono::duration<double> duration = end - start;  // ���������� ������� ���������� �����

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
