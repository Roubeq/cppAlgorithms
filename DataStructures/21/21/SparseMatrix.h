#include <iostream>

class SparseMatrix {
private:
    int* A;                 // Массив ненулевых элементов
    int* LI;                // Массив номеров строк
    int* LJ;                // Массив номеров столбцов
    int rows, cols, nnz;    // Количество строк, столбцов и ненулевых элементов

    bool isValidNumber(char* str, int& result);  // Вспомогательная функция

public:
    SparseMatrix();
    ~SparseMatrix();

    bool readAndPack(const char* filename);
    void rotate90();
    void unpackAndPrint() const;
    void printArrays();
    void fillManual(int r, int c, int count, const int* data, const int* rows, const int* cols);
    void extractData(int* aOut, int* liOut, int* ljOut) const;


    int getRows() const { return rows; }
    int getCols() const { return cols; }
    int getNnz() const { return nnz; }
    int getNonZeroCount() const { return nnz; }

    const int* getA() const { return A; }
    const int* getLI() const { return LI; }
    const int* getLJ() const { return LJ; }
};