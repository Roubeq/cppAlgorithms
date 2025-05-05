#include "SSSMatrix.h"

SSSMatrix::SSSMatrix() : rows(0), columns(0),
AD(nullptr), AU(nullptr), AL(nullptr), LJ(nullptr), LI(nullptr) { }

SSSMatrix::~SSSMatrix() {
    delete[] AD;
    delete[] AU;
    delete[] AL;
    delete[] LJ;
    delete[] LI;
}

void SSSMatrix::inputFromFileAndPackageMatrix(const char* filename) {
    std::ifstream file(filename);

    if (!file) {
        throw std::runtime_error("Ошибка открытия файла!");
    }

    file >> rows >> columns;
    if (file.fail()) {
        throw std::runtime_error("Не удалось прочитать размеры матрицы");
    }

    AD = new int[rows]();
    LI = new int[rows + 1]();

    int* auTemp = new int[rows * columns];
    int* alTemp = new int[rows * columns];
    int* ljTemp = new int[rows * columns];

    int auIndex = 0, alIndex = 0;
    LI[0] = 0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            int value;
            if (!(file >> value)) {
                delete[] auTemp;
                delete[] alTemp;
                delete[] ljTemp;
                throw std::runtime_error("Некорректные данные в матрице");
            }

            if (i == j) {
                AD[i] = value;
            }
            if (value != 0) {
                if (i < j) {
                    auTemp[auIndex] = value;
                    ljTemp[auIndex] = j;
                    auIndex++;
                }
                else if (i > j) {
                    alTemp[alIndex] = value;
                    alIndex++;
                }
            }
        }
        LI[i + 1] = auIndex;
    }

    AU = new int[auIndex];
    AL = new int[alIndex];
    LJ = new int[auIndex];

    for (int i = 0; i < auIndex; i++) {
        AU[i] = auTemp[i];
        LJ[i] = ljTemp[i];
    }

    for (int i = 0; i < alIndex; i++) {
        AL[i] = alTemp[i];
    }

    delete[] auTemp;
    delete[] alTemp;
    delete[] ljTemp;
}

int SSSMatrix::sumElements() {
    int sum = 0;
    if (AD) {
        for (int i = 0; i < rows; i++) {
            sum += AD[i];                   // Суммируем диагональные элементы
        }
    }
    if (AL) {
        for (int i = 0; i < LI[rows]; i++) {
            sum += AL[i];                   // Суммируем элементы ниже диагонали
        }
    }
    if (AU) {
        for (int i = 0; i < LI[rows]; i++) {
            sum += AU[i];                   // Суммируем элементы выше диагонали
        }
    }
    return sum;
}

void SSSMatrix::printArrays() const {
    std::cout << "Массив главной диагонали (AD): ";
    for (int i = 0; i < rows; ++i) {
        std::cout << AD[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Массив элементов выше диагонали (AU): ";
    for (int i = 0; i < LI[rows]; ++i) {
        std::cout << AU[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Массив элементов ниже диагонали (AL): ";
    for (int i = 0; i < LI[rows]; ++i) {
        std::cout << AL[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Массив индексов столбцов (LJ) для AU: ";
    for (int i = 0; i < LI[rows]; ++i) {
        std::cout << LJ[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Массив индексов начала строк (LI): ";
    for (int i = 0; i <= rows; ++i) {
        std::cout << LI[i] << " ";
    }
    std::cout << std::endl;
}

void SSSMatrix::fillManual(int rows, int cols, const int* ad, const int* au, const int* al, const int* lj, const int* li) {
    delete[] AD;
    delete[] AU;
    delete[] AL;
    delete[] LJ;
    delete[] LI;

    this->rows = rows;
    this->columns = cols;

    if (rows > 0 && ad != nullptr) {
        AD = new int[rows];
        for (int i = 0; i < rows; i++) {
            AD[i] = ad[i];
        }
    }
    else {
        AD = nullptr;
    }

    int au_al_count = (rows > 0 && li != nullptr) ? li[rows] : 0;

    if (au_al_count > 0 && au != nullptr) {
        AU = new int[au_al_count];
        for (int i = 0; i < au_al_count; i++) {
            AU[i] = au[i];
        }
    }
    else {
        AU = nullptr;
    }

    if (au_al_count > 0 && al != nullptr) {
        AL = new int[au_al_count];
        for (int i = 0; i < au_al_count; i++) {
            AL[i] = al[i];
        }
    }
    else {
        AL = nullptr;
    }

    if (au_al_count > 0 && lj != nullptr) {
        LJ = new int[au_al_count];
        for (int i = 0; i < au_al_count; i++) {
            LJ[i] = lj[i];
        }
    }
    else {
        LJ = nullptr;
    }

    if (rows > 0 && li != nullptr) {
        LI = new int[rows + 1];
        for (int i = 0; i <= rows; i++) {
            LI[i] = li[i];
        }
    }
    else {
        LI = nullptr;
    }
}

void SSSMatrix::extractData(int* ad, int* au, int* al, int* lj, int* li) const {
    for (int i = 0; i < rows; ++i) ad[i] = AD[i];
    int count = LI[rows];
    for (int i = 0; i < count; ++i) {
        au[i] = AU[i];
        al[i] = AL[i];
        lj[i] = LJ[i];
    }
    for (int i = 0; i <= rows; ++i) li[i] = LI[i];
}
