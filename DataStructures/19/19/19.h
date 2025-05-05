class CCSMatrix {
private:
    int lines, columns, notNullElements;
    double* A;
    int* LI;
    int* LJ;

public:
    CCSMatrix();
    ~CCSMatrix();

    // Запрещаем копирование и присваивание
    CCSMatrix(const CCSMatrix&) = delete;
    CCSMatrix& operator=(const CCSMatrix&) = delete;

    void readAndPack(const char* filename = "matrix.txt");
    void sortColumnsBySum();
    void printCCS() const;
    void reconstructAndPrint() const;
    void fillManual(int lines, int columns, int notNullElements,
        const double* A, const int* LI, const int* LJ);

    // Методы для тестирования
    int getLines() const { return lines; }
    int getColumns() const { return columns; }
    int getNonNullElements() const { return notNullElements; }
    const double* getA() const { return A; }
    const int* getLI() const { return LI; }
    const int* getLJ() const { return LJ; }

};