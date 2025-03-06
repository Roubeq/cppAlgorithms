#include <iostream>
#include <fstream>

using namespace std;

class SparseMatrix {
private:
    int* A;                 // Массив ненулевых элементов
    int* LI;                // Массив номеров строк ненулевых элементов
    int* LJ;                // Массив номеров столбцов ненулевых элементов
    int rows, cols, nnz;    // Количество строк, столбцов и ненулевых элементов
    const char* filename = "matrix.txt";

public:
    SparseMatrix() : rows(0), cols(0), nnz(0), A(nullptr), LI(nullptr), LJ(nullptr) {} // Конструктор

    ~SparseMatrix() {       // Деструктор, освобождает память
        delete[] A;
        delete[] LI;
        delete[] LJ;
    }

    // Функция проверяет, является ли строка числом, и конвертирует в int
    bool isValidNumber(char* str, int& result) {
        result = 0;
        bool negative = false;
        if (*str == '-') {                  // Проверка на отрицательное число
            negative = true;
            str++;
        }
        if (!*str) return false;            // Проверка на пустую строку

        while (*str) {                      // Обрабатываем каждый символ
            if (*str < '0' || *str > '9') return false;     // Если не цифра — ошибка
            result = result * 10 + (*str - '0');            // Преобразуем в число
            str++;
        }

        if (negative) result = -result;     // Учитываем знак
        return true;
    }

    // Чтение данных из файла и упаковка ненулевых элементов
    bool readAndPack() {
        ifstream file(filename);
        if (!file) {                        // Проверка на успешное открытие файла
            cerr << "Ошибка: не удалось открыть файл " << filename << endl;
            return false;
        }

        char* buffer = new char[1000];      // Динамический буфер для ввода чисел
        int temp;

        file >> buffer;
        if (!isValidNumber(buffer, temp) || (rows = temp) <= 0) {       // Читаем количество строк
            cerr << "Ошибка: некорректное значение строк" << endl;
            delete[] buffer;
            return false;
        }
        file >> buffer;
        if (!isValidNumber(buffer, temp) || (cols = temp) <= 0) {       // Читаем количество столбцов
            cerr << "Ошибка: некорректное значение столбцов" << endl;
            delete[] buffer;
            return false;
        }

        int capacity = (rows * cols) / 2;   // Оценка максимального количества ненулевых элементов
        A = new int[capacity];              // Динамическое выделение памяти
        LI = new int[capacity];
        LJ = new int[capacity];
        nnz = 0;

        for (int i = 0; i < rows; ++i) {    // Читаем матрицу из файла
            for (int j = 0; j < cols; ++j) {
                file >> buffer;
                if (!isValidNumber(buffer, temp)) {                     // Проверяем число
                    cerr << "Ошибка: некорректный элемент матрицы" << endl;
                    delete[] buffer;
                    return false;
                }
                if (temp != 0) {            // Записываем ненулевые элементы
                    A[nnz] = temp;
                    LI[nnz] = i;
                    LJ[nnz] = j;
                    nnz++;
                }
            }
        }

        if (file >> buffer) {               // Проверка на лишние данные
            cerr << "Ошибка: в файле лишние данные" << endl;
            delete[] buffer;
            return false;
        }

        delete[] buffer;                    // Освобождаем память
        file.close();
        return true;
    }

    // Функция поворота матрицы на 90 градусов по часовой стрелке
    void rotate90() {
        for (int k = 0; k < nnz; ++k) {
            int temp = LI[k];               // Меняем местами строки и столбцы
            LI[k] = LJ[k];
            LJ[k] = rows - 1 - temp;
        }
        int tempRows = rows;                // Меняем местами количество строк и столбцов
        rows = cols;
        cols = tempRows;
    }

    // Распаковка разреженной матрицы и вывод на экран
    void unpackAndPrint() const {
        int** fullMatrix = new int* [rows];         // Создаем массив строк
        for (int i = 0; i < rows; ++i) {
            fullMatrix[i] = new int[cols] {};       // Заполняем нулями
        }

        for (int k = 0; k < nnz; ++k) {             // Восстанавливаем ненулевые элементы
            if (LI[k] >= rows || LJ[k] >= cols) {
                cerr << "Ошибка: выход за границы при распаковке!" << endl;
                continue;
            }
            fullMatrix[LI[k]][LJ[k]] = A[k];        // Записываем ненулевые элементы
        }

        for (int i = 0; i < rows; ++i) {            // Выводим матрицу на экран
            for (int j = 0; j < cols; ++j) {
                cout << fullMatrix[i][j] << " ";
            }
            cout << endl;
        }

        for (int i = 0; i < rows; ++i) {            // Освобождаем память
            delete[] fullMatrix[i];
        }
        delete[] fullMatrix;
    }

    void printArrays() {
        cout << "массив ненулевых элементов (A): ";
        for (int i = 0; i < nnz; ++i) {
            cout << A[i] << " ";
        }
        cout << endl;

        cout << "массив строк (LI): ";
        for (int i = 0; i < nnz; ++i) {
            cout << LI[i] << " ";
        }
        cout << endl;

        cout << "массив столбцов (LJ): ";
        for (int i = 0; i < nnz; ++i) {
            cout << LJ[i] << " ";
        }
        cout << endl;
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    SparseMatrix matrix;
    if (!matrix.readAndPack()) {    // Читаем матрицу и проверяем на ошибки
        return 1;
    }
    matrix.rotate90();              // Поворачиваем матрицу
    matrix.unpackAndPrint();        // Выводим результат
    matrix.printArrays();
    return 0;
}
