#include <iostream>
#include <fstream>
using namespace std;

bool isNonDecreasing(int arr[], int size) {                // Проверка на неубывание массива
    for (int i = 1; i < size; i++) {
        if (arr[i] < arr[i - 1]) return false;
    }
    return true;
}

int main() {
    setlocale(LC_ALL, "Russian");
    ifstream fin("file.txt");                              // Открываем файл с входными данными
    if (!fin) {
        cout << "Не удалось открыть файл file.txt" << endl;
        return 1;
    }

    int N, M;

    if (!(fin >> N)) {                                     // Считываем размер массива A
        cout << "Ошибка: размер массива A не число." << endl;
        return 1;
    }
    if (N < 0) {                                           // Проверка на отрицательный размер
        cout << "Ошибка: размер массива A должен быть неотрицательным." << endl;
        return 1;
    }

    int* A = new int[N];                                   // Выделяем память под массив A

    for (int i = 0; i < N; i++) {                          // Считываем элементы массива A
        if (!(fin >> A[i])) {
            cout << "Ошибка: элемент массива A не число." << endl;
            delete[] A;
            return 1;
        }
    }

    if (!isNonDecreasing(A, N)) {                          // Проверка, отсортирован ли A
        cout << "Ошибка: массив A не отсортирован по неубыванию." << endl;
        delete[] A;
        return 1;
    }

    if (!(fin >> M)) {                                     // Считываем размер массива B
        cout << "Ошибка: размер массива B не число." << endl;
        delete[] A;
        return 1;
    }
    if (M < 0) {                                           // Проверка на отрицательный размер
        cout << "Ошибка: размер массива B должен быть неотрицательным." << endl;
        delete[] A;
        return 1;
    }

    int* B = new int[M];                                   // Выделяем память под массив B

    for (int i = 0; i < M; i++) {                          // Считываем элементы массива B
        if (!(fin >> B[i])) {
            cout << "Ошибка: элемент массива B не число." << endl;
            delete[] A;
            delete[] B;
            return 1;
        }
    }

    if (!isNonDecreasing(B, M)) {                          // Проверка, отсортирован ли B
        cout << "Ошибка: массив B не отсортирован по неубыванию." << endl;
        delete[] A;
        delete[] B;
        return 1;
    }

    fin.close();                                           // Закрываем файл

    cout << "\nМассив A (" << N << " элементов): ";        // Вывод исходных массивов
    for (int i = 0; i < N; i++) cout << A[i] << " ";
    cout << "\nМассив B (" << M << " элементов): ";
    for (int i = 0; i < M; i++) cout << B[i] << " ";

    cout << "\n\nПроцесс слияния:\n";

    int* C = new int[N + M];                               // Результирующий массив C

    int i = 0, j = 0, k = 0;
    while (i < N && j < M) {                               // Основной цикл слияния
        if (A[i] <= B[j]) {
            C[k] = A[i];
            cout << "C[" << k << "] = A[" << i << "] = " << A[i] << endl;
            i++;
        }
        else {
            C[k] = B[j];
            cout << "C[" << k << "] = B[" << j << "] = " << B[j] << endl;
            j++;
        }
        k++;
    }

    while (i < N) {                                        // Добавляем оставшиеся элементы A
        C[k] = A[i];
        cout << "C[" << k << "] = A[" << i << "] = " << A[i] << endl;
        i++; k++;
    }

    while (j < M) {                                        // Добавляем оставшиеся элементы B
        C[k] = B[j];
        cout << "C[" << k << "] = B[" << j << "] = " << B[j] << endl;
        j++; k++;
    }

    cout << "\nИтоговый отсортированный массив C (" << N + M << " элементов):\n";
    for (int i = 0; i < N + M; i++) {
        cout << C[i] << " ";
    }
    cout << endl;

    delete[] A;                                            // Освобождаем память
    delete[] B;
    delete[] C;

    return 0;
}