#include "SparseMatrix.h"
#include <fstream>
#include <iostream>

SparseMatrix::SparseMatrix() : rows(0), cols(0), nnz(0), A(nullptr), LI(nullptr), LJ(nullptr) {}

SparseMatrix::~SparseMatrix() {
    delete[] A;
    delete[] LI;
    delete[] LJ;
}

bool SparseMatrix::isValidNumber(char* str, int& result) {
    result = 0;
    bool negative = false;
    if (*str == '-') {
        negative = true;
        str++;
    }
    if (!*str) return false;

    while (*str) {
        if (*str < '0' || *str > '9') return false;
        result = result * 10 + (*str - '0');
        str++;
    }

    if (negative) result = -result;
    return true;
}

bool SparseMatrix::readAndPack(const char* filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
        return false;
    }

    char* buffer = new char[1000];
    int temp;

    // Чтение количества строк
    file.getline(buffer, 1000, ' ');
    if (!isValidNumber(buffer, temp) || (rows = temp) <= 0) {
        std::cerr << "Ошибка: некорректное значение строк: " << buffer << std::endl;
        delete[] buffer;
        return false;
    }

    // Чтение количества столбцов
    file.getline(buffer, 1000, ' ');
    if (!isValidNumber(buffer, temp) || (cols = temp) <= 0) {
        std::cerr << "Ошибка: некорректное значение столбцов: " << buffer << std::endl;
        delete[] buffer;
        return false;
    }

    int capacity = (rows * cols) / 2;
    A = new int[capacity];
    LI = new int[capacity];
    LJ = new int[capacity];
    nnz = 0;

    // Чтение элементов матрицы
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            file.getline(buffer, 1000, ' ');
            if (!isValidNumber(buffer, temp)) {
                std::cerr << "Ошибка: некорректный элемент матрицы на позиции (" << i << ", " << j << "): " << buffer << std::endl;
                delete[] buffer;
                return false;
            }
            if (temp != 0) {
                if (nnz >= capacity) {
                    std::cerr << "Ошибка: превышено допустимое количество ненулевых элементов!" << std::endl;
                    delete[] buffer;
                    return false;
                }
                A[nnz] = temp;
                LI[nnz] = i;
                LJ[nnz] = j;
                nnz++;
            }
        }
    }

    if (file.getline(buffer, 1000, ' ')) {
        std::cerr << "Ошибка: в файле лишние данные" << std::endl;
        delete[] buffer;
        return false;
    }

    delete[] buffer;
    file.close();
    return true;
}

void SparseMatrix::rotate90() {
    for (int k = 0; k < nnz; ++k) {
        int temp = LI[k];
        LI[k] = LJ[k];
        LJ[k] = rows - 1 - temp;
    }
    int tempRows = rows;
    rows = cols;
    cols = tempRows;
}

void SparseMatrix::unpackAndPrint() const {
    if (rows * cols > 1000000) {
        std::cout << "Матрица слишком большая для вывода (" << rows << "x" << cols << ")." << std::endl;
        return;
    }

    int** fullMatrix = new int* [rows];
    for (int i = 0; i < rows; ++i) {
        fullMatrix[i] = new int[cols] {};
    }

    for (int k = 0; k < nnz; ++k) {
        if (LI[k] >= rows || LJ[k] >= cols) {
            std::cerr << "Ошибка: выход за границы при распаковке!" << std::endl;
            continue;
        }
        fullMatrix[LI[k]][LJ[k]] = A[k];
    }

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << fullMatrix[i][j] << " ";
        }
        std::cout << std::endl;
    }

    for (int i = 0; i < rows; ++i) {
        delete[] fullMatrix[i];
    }
    delete[] fullMatrix;
}

void SparseMatrix::printArrays() {
    std::cout << "массив ненулевых элементов (A): ";
    for (int i = 0; i < nnz; ++i) {
        std::cout << A[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "массив строк (LI): ";
    for (int i = 0; i < nnz; ++i) {
        std::cout << LI[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "массив столбцов (LJ): ";
    for (int i = 0; i < nnz; ++i) {
        std::cout << LJ[i] << " ";
    }
    std::cout << std::endl;
}

void SparseMatrix::fillManual(int r, int c, int count, const int* data, const int* rowIdx, const int* colIdx) {

    // Очистка, если уже что-то есть
    delete[] A;
    delete[] LI;
    delete[] LJ;

    rows = r;
    cols = c;
    nnz = count;

    A = new int[nnz];
    LI = new int[nnz];
    LJ = new int[nnz];

    for (int i = 0; i < nnz; ++i) {
        A[i] = data[i];
        LI[i] = rowIdx[i];
        LJ[i] = colIdx[i];
    }
}

void SparseMatrix::extractData(int* aOut, int* liOut, int* ljOut) const {
    for (int i = 0; i < nnz; ++i) {
        aOut[i] = A[i];
        liOut[i] = LI[i];
        ljOut[i] = LJ[i];
    }
}