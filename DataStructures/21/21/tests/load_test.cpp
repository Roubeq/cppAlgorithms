#include <iostream>
#include <windows.h>
#include <chrono>
#include "../SparseMatrix.h"
#include <unordered_set>


class SystemMonitor {                              // ����� ��� ����������� ������� (������, CPU)
public:
    void printMemoryStats() {
        MEMORYSTATUSEX memInfo;
        memInfo.dwLength = sizeof(memInfo);
        if (GlobalMemoryStatusEx(&memInfo)) {
            std::cout << "Memory Load: " << memInfo.dwMemoryLoad << "%\n";
            std::cout << "Available Physical Memory: " << memInfo.ullAvailPhys / (1024 * 1024) << " MB\n";  // ��������� ���������� ������
        }
    }

    void printCPUUsage() {                         // ����� ��� ������ ���������� �� ������������� CPU
        FILETIME idle1, kernel1, user1, idle2, kernel2, user2;
        GetSystemTimes(&idle1, &kernel1, &user1);  // �������� ��������� ����� �������
        Sleep(100);                                // �������� �� 100 ��
        GetSystemTimes(&idle2, &kernel2, &user2);  // �������� �������� ����� �������

        ULONGLONG sys1 = toULL(kernel1) + toULL(user1);  // ������� ��������� ����� ������������� ���������� �� ��������� ������
        ULONGLONG sys2 = toULL(kernel2) + toULL(user2);  // ������� ��������� ����� ������������� ���������� �� �������� ������
        ULONGLONG idleDiff = toULL(idle2) - toULL(idle1);// ������� �� ������� �������
        ULONGLONG sysDiff = sys2 - sys1;                 // ������� �� ������� ���������� ����������

        if (sysDiff != 0) {
            double cpu = (1.0 - (double)idleDiff / sysDiff) * 100.0;  // ������������ �������� ����������
            std::cout << "CPU Usage: " << cpu << " %\n";              // ������� �������� ����������
        }
    }

private:
    ULONGLONG toULL(FILETIME t) {                                 // �������������� FILETIME � ULONGLONG ��� �������� �������
        return ((ULONGLONG)t.dwHighDateTime << 32) | t.dwLowDateTime;
    }
};

void performanceTestSparse() {
    const int max_size = 1000;  // ������������ ������ ������� ��� ������������
    const int step = 100;       // ��� ���������� ������� �������

    SystemMonitor monitor;      // ������� ������ ����������� �������

    for (int size = 100; size <= max_size; size += step) {         // ��������� ����� ��� ��������� �������� ������
        int total = size * size;// ����� ���������� ��������� � �������
        int nnz = total / 2;    // ������������ ���������� ���������� ��������� ���������

        int* A = new int[nnz];
        int* LI = new int[nnz];
        int* LJ = new int[nnz];

        std::unordered_set<long long> used; // ��������� ��� ������������ �������������� �������
        int count = 0;
        while (count < nnz) {
            int r = rand() % size;          // ��������� ��������� ������
            int c = rand() % size;          // ��������� ���������� �������
            long long key = static_cast<long long>(r) * size + c;  // �������� ����������� ����� ��� �������

            if (used.count(key)) continue;  // ���� ������� ��� ��������������, ���������� ����� ����������
            used.insert(key);               // ��������� ���������� ������� � ���������

            A[count] = rand() % 100 + 1;  // ���������� ���������� �������� ��������� ���������
            LI[count] = r;                // ���������� ������� ������
            LJ[count] = c;                // ���������� ������� �������
            ++count;
        }

        std::cout << "\nTest: " << size << "x" << size << ", nnz = " << nnz << std::endl;  // ������� ������� ��������� �����

        std::cout << "[Before test]\n";  // ���������� �� �����
        monitor.printMemoryStats();      // ������ ���������� ������
        monitor.printCPUUsage();         // ������ ���������� CPU

        auto start = std::chrono::high_resolution_clock::now();  // �������� ����� ������ �����

        SparseMatrix mat;                            // ������� ������ SparseMatrix
        mat.fillManual(size, size, nnz, A, LI, LJ);  // ��������� �������
        mat.rotate90();                              // ������������ ������� �� 90 ��������

        auto end = std::chrono::high_resolution_clock::now();   // �������� ����� ����� �����
        std::chrono::duration<double> time = end - start;       // ������ ������� ���������� �����

        std::cout << "Time: " << time.count() << " seconds\n";  // ������� ����� ����������

        std::cout << "[After test]\n";  // ���������� ����� �����
        monitor.printMemoryStats();     // ������ ���������� ������
        monitor.printCPUUsage();        // ������ ���������� CPU

        delete[] A;
        delete[] LI;
        delete[] LJ;
    }

    std::cout << "\nLoad test completed.\n";  // ��������� �� ��������� �����
}

int main() {
    setlocale(LC_ALL, "Russian");  // ������������� ������ ��� ������� ��������
    SetConsoleOutputCP(CP_UTF8);   // ������������� ��������� UTF-8 ��� ������ � �������

    performanceTestSparse();       // ��������� ������������ ������������������

    return 0;
}
