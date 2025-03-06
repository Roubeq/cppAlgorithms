#include <iostream>
#include <fstream>

using namespace std;

class SSSMatrix {
private:
    int rows, columns;
    int* AD;                                    // Главная диагональ
    int* AU;                                    // Элементы выше диагонали (слева направо, построчно)
    int* AL;                                    // Элементы ниже диагонали (сверху вниз, по столбцам)
    int* LJ;                                    // Индексы столбцов (для AU) или строк (для AL)
    int* LI;                                    // Индексы начала строк (для AU) или столбцов (для AL)

public:
    SSSMatrix() : rows(0), columns(0),
        AD(nullptr), AU(nullptr), AL(nullptr), LJ(nullptr), LI(nullptr) { }

    ~SSSMatrix() {
        delete[] AD;
        delete[] AU;
        delete[] AL;
        delete[] LJ;
        delete[] LI;
    }

    void inputFromFileAndPackageMatrix() {
        ifstream file("SSSMatrix.txt");

        if (!file) {
            cerr << "Ошибка открытия файла!";
            return;
        }

        file >> rows >> columns;
        int value;
        AD = new int[rows]();                                   // Выделяем память под диагональные элементы
        LI = new int[rows + 1]();                               // Индексы начала строк (размер на 1 больше)

        int* auTemp = new int[rows * columns];
        int* alTemp = new int[rows * columns];
        int* ljTemp = new int[rows * columns];

        int auIndex = 0, alIndex = 0;
        LI[0] = 0;

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                file >> value;
                if (file.fail()) {
                    cout << "Матрица некорректно введена, проверьте и исправьте данные в файле.";
                    exit(1);
                }
                    if (i == j) {
                        AD[i] = value;                              // Заполняем диагональ
                    }
                    if (value != 0) {
                        if (i < j) {                                // Над диагональю
                            auTemp[auIndex] = value;
                            ljTemp[auIndex] = j;
                            auIndex++;
                        }
                        else if (i > j) {                           // Под диагональю
                            alTemp[alIndex] = value;
                            alIndex++;
                        }
                    }
            }
            LI[i + 1] = auIndex;                // Запоминаем границу строки
        }

        AU = new int[auIndex];                  // Выделяем память под AU
        AL = new int[alIndex];                  // Выделяем память под AL
        LJ = new int[auIndex];                  // Выделяем память под LJ

        for (int i = 0; i < auIndex; i++) {
            AU[i] = auTemp[i];
            LJ[i] = ljTemp[i];
        }

        for (int i = 0; i < alIndex; i++) {
            AL[i] = alTemp[i];
        }

        delete[] auTemp;                        // Освобождаем временные массивы
        delete[] alTemp;
        delete[] ljTemp;
    }

    int sumElements() {
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

    void printArrays() const {
        cout << "Массив главной диагонали (AD): ";
        for (int i = 0; i < rows; ++i) {
            cout << AD[i] << " ";
        }
        cout << endl;

        cout << "Массив элементов выше диагонали (AU): ";
        for (int i = 0; i < LI[rows]; ++i) {    // Используем LI[rows] как границу
            cout << AU[i] << " ";
        }
        cout << endl;

        cout << "Массив элементов ниже диагонали (AL): ";
        for (int i = 0; i < LI[rows]; ++i) {
            cout << AL[i] << " ";
        }
        cout << endl;

        cout << "Массив индексов столбцов (LJ) для AU: ";
        for (int i = 0; i < LI[rows]; ++i) {
            cout << LJ[i] << " ";
        }
        cout << endl;

        cout << "Массив индексов начала строк (LI): ";
        for (int i = 0; i <= rows; ++i) {       // LI на 1 больше, чем количество строк
            cout << LI[i] << " ";
        }
        cout << endl;
    }

};

int main() {
    setlocale(LC_ALL, "Russian");
    SSSMatrix A;
    A.inputFromFileAndPackageMatrix();
    A.printArrays();
    cout << A.sumElements() << endl;            // Выводим сумму элементов
}