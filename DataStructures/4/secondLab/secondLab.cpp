#include <iostream>
#include <fstream>

using namespace std;

class CCSMatrix {
private:
    int lines, columns, notNullElements;    // Количество строк, столбцов и ненулевых элементов
    double* A;                              // Значения ненулевых элементов
    int* LI;                                // Строковые индексы ненулевых элементов
    int* LJ;                                // Указатели на начало каждого столбца в A и LI

public:
    CCSMatrix() : lines(0), columns(0), notNullElements(0), A(nullptr), LI(nullptr), LJ(nullptr) {}

    ~CCSMatrix() {
        delete[] A;
        delete[] LI;
        delete[] LJ;
    }

    void readAndPack() {
        ifstream file("matrix.txt");
        if (!file) {
            cout << "Ошибка открытия файла!" << endl;
            exit(1);
        }

        file >> lines >> columns;
        if (file.fail()) {
            cout << "Матрица некорректно введена, проверьте и исправьте данные в файле.";
            exit(1);
        }


        LJ = new int[columns + 1]();                // Инициализация массива указателей столбцов
        int* colCount = new int[columns]();         // Подсчет ненулевых элементов в каждом столбце
        int tempValue, count = 0;

        for (int i = 0; i < lines; i++) {
            for (int j = 0; j < columns; j++) {
                file >> tempValue;
                if (file.fail()) {
                    cout << "Матрица некорректно введена, проверьте и исправьте данные в файле.";
                    exit(1);
                }
                if (tempValue != 0) {
                    colCount[j]++;
                    count++;
                }
            }
        }

        if (count > 0.5 * (lines * columns)) {
            cout << "Матрица не является разреженной";
            exit(1);
        }

        notNullElements = count;                    // Общее количество ненулевых элементов
        A = new double[notNullElements];
        LI = new int[notNullElements];

        LJ[0] = 0;
        for (int j = 0; j < columns; j++) {
            LJ[j + 1] = LJ[j] + colCount[j];        // Заполняем массив LJ
        }

        delete[] colCount;

        file.clear();
        file.seekg(0, ios::beg);
        file >> lines >> columns;

        int* currentPos = new int[columns]();               // Текущая позиция вставки для каждого столбца

        for (int i = 0; i < lines; i++) {
            for (int j = 0; j < columns; j++) {
                file >> tempValue;
                if (tempValue != 0) {
                    int index = LJ[j] + currentPos[j];
                    A[index] = tempValue;                   // Заполняем массив значений
                    LI[index] = i;                          // Заполняем массив строковых индексов
                    currentPos[j]++;
                }
            }
        }

        delete[] currentPos;
        file.close();
    }


    void sortColumnsBySum() {
        double* colSums = new double[columns]();            // Массив для хранения сумм элементов каждого столбца
        int* colIndices = new int[columns];                 // Индексы столбцов
        for (int j = 0; j < columns; j++) {
            colIndices[j] = j;
            for (int k = LJ[j]; k < LJ[j + 1]; k++) {
                colSums[j] += A[k];                         // Подсчитываем сумму элементов столбца
            }
        }

        for (int i = 0; i < columns - 1; i++) {
            for (int j = 0; j < columns - i - 1; j++) {
                if (colSums[j] > colSums[j + 1]) {
                    double tempSum = colSums[j];            // Меняем местами colSums
                    colSums[j] = colSums[j + 1];
                    colSums[j + 1] = tempSum;

                    int tempIndex = colIndices[j];         // Меняем местами colIndices
                    colIndices[j] = colIndices[j + 1];
                    colIndices[j + 1] = tempIndex;
                }
            }
        }

        double* newA = new double[notNullElements];         // Новый массив значений
        int* newLI = new int[notNullElements];              // Новый массив строковых индексов
        int* newLJ = new int[columns + 1];                  // Новый массив указателей столбцов
        newLJ[0] = 0;

        int pos = 0;
        for (int newJ = 0; newJ < columns; newJ++) {
            int oldJ = colIndices[newJ];
            for (int k = LJ[oldJ]; k < LJ[oldJ + 1]; k++) {
                newA[pos] = A[k];
                newLI[pos] = LI[k];
                pos++;
            }
            newLJ[newJ + 1] = pos;                          // Обновляем указатели столбцов
        }

        delete[] A;
        delete[] LI;
        delete[] LJ;
        delete[] colSums;
        delete[] colIndices;

        A = newA;
        LI = newLI;
        LJ = newLJ;
    }

    void printCCS() {
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

    void reconstructAndPrint() {
        cout << "Восстановленная матрица:" << endl;
        for (int i = 0; i < lines; i++) {
            for (int j = 0; j < columns; j++) {
                double value = 0;
                for (int k = LJ[j]; k < LJ[j + 1]; k++) {
                    if (LI[k] == i) {
                        value = A[k];                       // Восстанавливаем значение из CRS
                        break;
                    }
                }
                cout << value << " ";
            }
            cout << endl;
        }
    }

};

int main() {
    setlocale(LC_ALL, "Russian");
    CCSMatrix matrix;
    matrix.readAndPack();
    matrix.printCCS();
    matrix.sortColumnsBySum();
    cout << "После сортировки столбцов:" << endl;
    matrix.printCCS();
    matrix.reconstructAndPrint();
    return 0;
}
