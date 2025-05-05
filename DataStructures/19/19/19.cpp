#include "19.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cmath>

using namespace std;

CCSMatrix::CCSMatrix() : lines(0), columns(0), notNullElements(0), A(nullptr), LI(nullptr), LJ(nullptr) {}

CCSMatrix::~CCSMatrix() {
    delete[] A;
    delete[] LI;
    delete[] LJ;
}

void CCSMatrix::readAndPack(const char* filename) {

    delete[] A; A = nullptr;
    delete[] LI; LI = nullptr;
    delete[] LJ; LJ = nullptr;

    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Ошибка открытия файла: " + string(filename));
    }

    file >> lines >> columns;
    if (file.fail() || lines <= 0 || columns <= 0) {
        throw runtime_error("Некорректные размеры матрицы");
    }

    // Первый проход: подсчет ненулевых элементов
    LJ = new int[columns + 1]();
    int* colCount = new int[columns]();
    int count = 0;
    double tempValue;

    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < columns; j++) {
            if (!(file >> tempValue)) {
                delete[] LJ;
                delete[] colCount;
                throw runtime_error("Ошибка чтения элемента матрицы");
            }

            if (fabs(tempValue) > 1e-10) { // Учитываем числа с плавающей точкой
                colCount[j]++;
                count++;
            }
        }
    }

    // Проверка на разреженность
    if (count > 0.5 * lines * columns) {
        delete[] LJ;
        delete[] colCount;
        throw runtime_error("Матрица не является разреженной");
    }

    if (count == 0) {
        delete[] LJ;
        delete[] colCount;
        throw runtime_error("Матрица не содержит ненулевых элементов");
    }

    notNullElements = count;
    A = new double[notNullElements];
    LI = new int[notNullElements];

    // Заполнение LJ
    LJ[0] = 0;
    for (int j = 0; j < columns; j++) {
        LJ[j + 1] = LJ[j] + colCount[j];
    }
    delete[] colCount;

    // Второй проход: заполнение A и LI
    file.clear();
    file.seekg(0);
    file >> lines >> columns; // Пропускаем размеры

    int* currentPos = new int[columns]();
    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < columns; j++) {
            file >> tempValue;
            if (fabs(tempValue) > 1e-10) {
                int index = LJ[j] + currentPos[j];
                A[index] = tempValue;
                LI[index] = i;
                currentPos[j]++;
            }
        }
    }
    delete[] currentPos;
    file.close();
}

void CCSMatrix::sortColumnsBySum() {
    if (columns == 0 || notNullElements == 0 || A == nullptr || LI == nullptr || LJ == nullptr) {
        throw std::runtime_error("Матрица не инициализирована или повреждена");
    }

    double* colSums = new double[columns]();
    int* colIndices = new int[columns];

    // Вычисление сумм столбцов
    for (int j = 0; j < columns; j++) {
        colIndices[j] = j;
        for (int k = LJ[j]; k < LJ[j + 1]; k++) {
            colSums[j] += A[k];
        }
    }

    // Сортировка пузырьком
    for (int i = 0; i < columns - 1; i++) {
        for (int j = 0; j < columns - i - 1; j++) {
            if (colSums[j] > colSums[j + 1]) {
                std::swap(colSums[j], colSums[j + 1]);
                std::swap(colIndices[j], colIndices[j + 1]);
            }
        }
    }

    // Новые массивы для перестроенной матрицы
    double* newA = nullptr;
    int* newLI = nullptr;
    int* newLJ = nullptr;

    try {
        newA = new double[notNullElements];
        newLI = new int[notNullElements];
        newLJ = new int[columns + 1];
        newLJ[0] = 0;

        int pos = 0;
        for (int newJ = 0; newJ < columns; newJ++) {
            int oldJ = colIndices[newJ];
            for (int k = LJ[oldJ]; k < LJ[oldJ + 1]; k++) {
                newA[pos] = A[k];
                newLI[pos] = LI[k];
                pos++;
            }
            newLJ[newJ + 1] = pos;
        }
    }
    catch (...) {
        delete[] colSums;
        delete[] colIndices;
        delete[] newA;
        delete[] newLI;
        delete[] newLJ;
        throw; // Пробросить исключение дальше
    }

    // Освобождение старой памяти
    delete[] A;
    delete[] LI;
    delete[] LJ;

    // Назначаем новые массивы
    A = newA;
    LI = newLI;
    LJ = newLJ;

    // Освобождение вспомогательной памяти
    delete[] colSums;
    delete[] colIndices;
}

void CCSMatrix::printCCS() const {
    if (notNullElements == 0) {
        cout << "Матрица пуста" << endl;
        return;
    }

    cout << "A: ";
    for (int i = 0; i < notNullElements; i++) {
        cout << A[i] << " ";
    }
    cout << endl;

    cout << "LI: ";
    for (int i = 0; i < notNullElements; i++) {
        cout << LI[i] << " ";
    }
    cout << endl;

    cout << "LJ: ";
    for (int i = 0; i <= columns; i++) {
        cout << LJ[i] << " ";
    }
    cout << endl;
}

void CCSMatrix::reconstructAndPrint() const {
    if (lines == 0 || columns == 0) {
        cout << "Матрица не инициализирована" << endl;
        return;
    }

    cout << "Восстановленная матрица:" << endl;
    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < columns; j++) {
            double value = 0.0;
            for (int k = LJ[j]; k < LJ[j + 1]; k++) {
                if (LI[k] == i) {
                    value = A[k];
                    break;
                }
            }
            cout << value << "\t";
        }
        cout << endl;
    }
}

void CCSMatrix::fillManual(int l, int c, int nnz,
    const double* a, const int* li, const int* lj) {
    if (nnz == 0) {
        throw std::runtime_error("Matrix contains only zeros");
    }
    delete[] A;
    delete[] LI;
    delete[] LJ;

    lines = l;
    columns = c;
    notNullElements = nnz;

    A = new double[nnz];
    LI = new int[nnz];
    LJ = new int[c + 1];

    for (int i = 0; i < nnz; ++i) {
        A[i] = a[i];
        LI[i] = li[i];
    }
    for (int i = 0; i <= c; ++i) {
        LJ[i] = lj[i];
    }
}
