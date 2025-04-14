#include <iostream>
#include <fstream>
using namespace std;

// Получение количества цифр в числе
int getLength(int x) {
    if (x == 0) return 1;               // Если число равно 0, длина = 1
    int len = 0;
    while (x > 0) {                     // Пока число не закончится
        len++;
        x /= 10;                        // Уменьшаем число на 1 разряд
    }
    return len;
}

// Получение цифры по позиции (0 - старший разряд)
int getDigit(int x, int pos, int maxLen) {
    int len = getLength(x);             // Получаем длину числа
    for (int i = 0; i < maxLen - pos - 1; i++) {
        x /= 10;                        // Сдвигаем число влево
    }
    return x % 10;                      // Возвращаем цифру на позиции pos
}

// Лексикографическое сравнение двух чисел
bool lexLess(int a, int b) {
    // Массивы для хранения цифр
    int digitsA[10], digitsB[10];
    int lenA = 0, lenB = 0;

    // Разбиваем a на цифры
    if (a == 0) digitsA[lenA++] = 0;    // Если a == 0, сразу записываем цифру 0
    while (a > 0) {                     // Разбиваем число на цифры
        digitsA[lenA++] = a % 10;
        a /= 10;
    }
    // Разворачиваем цифры
    for (int i = 0; i < lenA / 2; i++) {
        swap(digitsA[i], digitsA[lenA - i - 1]);
    }

    // Разбиваем b на цифры
    if (b == 0) digitsB[lenB++] = 0;    // Если b == 0, сразу записываем цифру 0
    while (b > 0) {                     // Разбиваем число на цифры
        digitsB[lenB++] = b % 10;
        b /= 10;
    }
    // Разворачиваем цифры
    for (int i = 0; i < lenB / 2; i++) {
        swap(digitsB[i], digitsB[lenB - i - 1]);
    }

    // Сравниваем цифры
    int minLen = min(lenA, lenB);                   // Берем минимальную длину чисел
    for (int i = 0; i < minLen; i++) {
        if (digitsA[i] < digitsB[i]) return true;   // Если цифра из a меньше, возвращаем true
        if (digitsA[i] > digitsB[i]) return false;  // Если цифра из a больше, возвращаем false
    }

    return lenA < lenB;
}

// Лексикографическая быстрая сортировка
void lexQuickSort(int* arr, int low, int high) {
    if (low < high) {
        int pivot = arr[high];            // Опорный элемент
        int i = (low - 1);

        for (int j = low; j < high; j++) {
            if (lexLess(arr[j], pivot)) { // Используем лексикографическое сравнение
                i++;
                swap(arr[i], arr[j]);
            }
        }
        swap(arr[i + 1], arr[high]);      // Перемещаем опорный элемент в правильное место
        int pi = i + 1;

        lexQuickSort(arr, low, pi - 1);   // Рекурсивная сортировка левой части
        lexQuickSort(arr, pi + 1, high);  // Рекурсивная сортировка правой части
    }
}

// Быстрая сортировка (QuickSort)
void quickSort(int* arr, int low, int high) {
    if (low < high) {
        int pivot = arr[high];          // Опорный элемент
        int i = (low - 1);

        for (int j = low; j < high; j++) {
            if (arr[j] <= pivot) {      // Если текущий элемент меньше или равен опорному
                i++;
                swap(arr[i], arr[j]);
            }
        }
        swap(arr[i + 1], arr[high]);    // Перемещаем опорный элемент в нужное место
        int pi = i + 1;

        quickSort(arr, low, pi - 1);    // Рекурсивная сортировка левой части
        quickSort(arr, pi + 1, high);   // Рекурсивная сортировка правой части
    }
}

// Функция для считывания камней из файла
int* readFromFile(int& n) {
    ifstream fin("file.txt");
    if (!fin) {                         // Если файл не открыт
        cout << "Ошибка открытия файла.\n";
        exit(1);
    }

    n = 0;
    int temp;
    int capacity = 10;                  // Начальная емкость массива
    int* stones = new int[capacity];    // Массив для хранения камней

    while (true) {
        fin >> ws;                      // Пропускаем пробелы
        if (fin.eof()) break;           // Если достигнут конец файла, выходим

        if (!(fin >> temp)) {           // Если данные не число
            cout << "Ошибка: файл содержит недопустимые данные (не число).\n";
            delete[] stones;
            exit(1);
        }

        if (n >= capacity) {            // Если массив заполнен, увеличиваем емкость
            capacity *= 2;
            int* newArr = new int[capacity];
            for (int i = 0; i < n; i++) {
                newArr[i] = stones[i];
            }
            delete[] stones;
            stones = newArr;
        }

        stones[n++] = temp;             // Добавляем число в массив
    }

    if (n == 0) {                       // Если в файле нет данных
        cout << "Файл не содержит чисел.\n";
        delete[] stones;
        exit(1);
    }

    cout << "Считанные камни: ";
    for (int i = 0; i < n; i++) {
        cout << stones[i] << " ";       // Выводим считанные данные
    }
    cout << endl;

    return stones;
}

// Функция для вывода массива
void printArray(int* arr, int n) {
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";           // Выводим элементы массива
    }
    cout << endl;
}

// Функция для разбиения камней
bool divideStones(int* stones, int n) {
    cout << "Сортировка камней...\n";
    quickSort(stones, 0, n - 1);         // Сортировка камней по возрастанию

    cout << "Отсортированные камни: ";
    printArray(stones, n);               // Выводим отсортированные камни

    int totalWeight = 0;
    for (int i = 0; i < n; i++) {
        totalWeight += stones[i];        // Суммируем общий вес камней
    }

    cout << "Общий вес всех камней: " << totalWeight << endl;

    int sumLeft = 0;
    for (int i = 0; i < n; i++) {
        sumLeft += stones[i];                  // Добавляем камень в левую кучку
        int sumRight = totalWeight - sumLeft;  // Считаем вес правой кучки

        cout << "\nШаг " << i + 1 << ":\n";
        cout << "Добавили камень " << stones[i] << " в левую кучку.\n";
        cout << "Вес левой кучки: " << sumLeft << ", правой кучки: " << sumRight << endl;

        if (sumLeft <= 1.5 * sumRight && sumRight <= 1.5 * sumLeft) {  // Условие разбиения
            cout << "Разбиение возможно на этом шаге.\n";

            int* leftPile = new int[i + 1];
            int* rightPile = new int[n - i - 1];

            for (int j = 0; j <= i; j++) {
                leftPile[j] = stones[j];           // Заполняем левую кучку
            }
            for (int j = i + 1; j < n; j++) {
                rightPile[j - i - 1] = stones[j];  // Заполняем правую кучку
            }

            cout << "Левая кучка: ";
            printArray(leftPile, i + 1);           // Выводим левую кучку
            cout << "Правая кучка: ";
            printArray(rightPile, n - i - 1);      // Выводим правую кучку

            delete[] leftPile;
            delete[] rightPile;
            return true;
        }
        else {
            cout << "Разбиение пока невозможно на этом шаге.\n";
        }
    }

    cout << "\nРазбиение невозможно для заданных данных.\n";  // Если не удалось разбить
    return false;
}

int main() {
    setlocale(LC_ALL, "Russian");   // Устанавливаем русскую локализацию
    int n;
    int* stones = readFromFile(n);  // Считываем камни из файла
    divideStones(stones, n);        // Пытаемся разделить камни
    delete[] stones;                // Освобождаем память
    return 0;
}
