#include <iostream>
#include <fstream>

class SSSMatrix {
private:
    int rows, columns;
    int* AD;                                    // Главная диагональ
    int* AU;                                    // Элементы выше диагонали
    int* AL;                                    // Элементы ниже диагонали
    int* LJ;                                    // Индексы столбцов (для AU) или строк (для AL)
    int* LI;                                    // Индексы начала строк (для AU) или столбцов (для AL)

public:
    SSSMatrix();
    ~SSSMatrix();

    void inputFromFileAndPackageMatrix(const char* filename);
    int sumElements();
    void printArrays() const;
    void extractData(int* ad, int* au, int* al, int* lj, int* li) const;
    void fillManual(int rows, int cols, const int* ad, const int* au, const int* al, const int* lj, const int* li);

    int getRows() const { return rows; }
    int getColumns() const { return columns; }
    const int* getAD() const { return AD; }
    const int* getAU() const { return AU; }
    const int* getAL() const { return AL; }
    const int* getLJ() const { return LJ; }
    const int* getLI() const { return LI; }
    int getAUCount() const { return LI[rows]; }
};
