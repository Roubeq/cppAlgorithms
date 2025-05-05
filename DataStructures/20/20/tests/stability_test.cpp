#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include "../SSSMatrix.h"

bool arraysEqual(const int* a1, const int* a2, int size) {          // ������� ��� ��������� ���� ��������
    for (int i = 0; i < size; ++i) {
        if (a1[i] != a2[i]) return false;
    }
    return true;                                      // ������� ���������
}

void stabilityTestSSS() {                             // ������� ��� ������������ ������������ SSSMatrix
    const int size = 100;                             // ������ �������
    const int maxOffDiagonal = size * (size - 1) / 2; // ������������ ���������� ��������� ��� ���������
    const int au_al_count = maxOffDiagonal / 4;       // ���������� ��������� AU/AL
    const int iterations = 1000;                      // ���������� �������� ��� ������������

    std::cout << "=== SSSMatrix Stability Test ===\n";
    std::cout << "Size: " << size << "x" << size << ", AU/AL count: " << au_al_count << ", Iterations: " << iterations << "\n";  // English message

    int* AD_original = new int[size];
    int* AU_original = new int[au_al_count];
    int* AL_original = new int[au_al_count];
    int* LJ_original = new int[au_al_count];
    int* LI_original = new int[size + 1];

    for (int i = 0; i < size; ++i)
        AD_original[i] = rand() % 100 + 1;

    for (int i = 0; i < au_al_count; ++i) {
        AU_original[i] = rand() % 100 + 1;
        AL_original[i] = rand() % 100 + 1;
        LJ_original[i] = rand() % size;
    }

    LI_original[0] = 0;                             // LI ������ ���� ������ ������������ �������������������, ��������������� �� au_al_count
    for (int i = 1; i <= size; ++i) {
        LI_original[i] = std::min(au_al_count, LI_original[i - 1] + (rand() % 2));
    }

    LI_original[size] = au_al_count;                // ��������, ��� ��������� ������� LI ����� au_al_count

    for (int it = 1; it <= iterations; ++it) {                          // ���������� ����� ��� ��������� ����� ��������
        SSSMatrix mat;
        mat.fillManual(size, size, AD_original, AU_original, AL_original, LJ_original, LI_original);

        int* AD_test = new int[size];                                   // ���������� ������
        int* AU_test = new int[au_al_count];
        int* AL_test = new int[au_al_count];
        int* LJ_test = new int[au_al_count];
        int* LI_test = new int[size + 1];

        mat.extractData(AD_test, AU_test, AL_test, LJ_test, LI_test);

        assert(arraysEqual(AD_original, AD_test, size));                // ��������� �������� � ����������� ������
        assert(arraysEqual(AU_original, AU_test, au_al_count));
        assert(arraysEqual(AL_original, AL_test, au_al_count));
        assert(arraysEqual(LJ_original, LJ_test, au_al_count));
        assert(arraysEqual(LI_original, LI_test, size + 1));

        delete[] AD_test;
        delete[] AU_test;
        delete[] AL_test;
        delete[] LJ_test;
        delete[] LI_test;

        if (it % 100 == 0)                                              // ����� ���������� �� ������ 100 ��������
            std::cout << "Iteration " << it << " passed.\n"; 
    }

    std::cout << "Stability test passed successfully.\n";

    delete[] AD_original;
    delete[] AU_original;
    delete[] AL_original;
    delete[] LJ_original;
    delete[] LI_original;
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));          // ������������� ���������� ��������� �����
    stabilityTestSSS();
    return 0;
}
