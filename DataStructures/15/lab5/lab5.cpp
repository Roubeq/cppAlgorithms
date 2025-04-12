#include <iostream>
#include <fstream>
#include <Windows.h>

using namespace std;

const int INITIAL_SIZE = 10;                             // Начальный размер хеш-таблицы
const int MAX_CHAIN_LENGTH = 3;                          // Максимальная длина цепочки до рехеширования

// Функция для сравнения строк
bool stringsEqual(const char* a, const char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {               // Проходим по символам строк
        if (a[i] != b[i]) return false;                  // Если символы разные, строки разные
        i++;
    }
    return a[i] == b[i];                                 // Проверяем, что строки одинаковой длины
}

// Функция для копирования строки с ограничением по длине
void stringCopy(char* dest, const char* src, int maxLen) {
    int i = 0;
    while (src[i] != '\0' && i < maxLen - 1) {           // Копируем до конца строки или до лимита длины
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';                                      // Завершаем строку
}

// Хеш-функция для строки
int hashU(char* v, int M) {
    int h = 0, a = 31415, b = 27183;                     // Начальные значения для хеширования
    for (h = 0 ;*v != 0; v++, a = a * b % (M - 1)) {          // Проходим по всем символам строки
        h = (a * h + *v) % M;                            // Обновляем хеш на основе символа
    }
    return (h < 0) ? (h + M) : h;                        // Возвращаем положительный хеш
}

struct Node {
    char key[20];                                        // Ключ (строка) для элемента
    Node* next;                                          // Указатель на следующий элемент в цепочке

    Node(const char* k) : next(nullptr) {                // Конструктор для инициализации нового узла
        stringCopy(key, k, 20);                          // Копируем ключ в узел
    }
};

class HashTable {
private:
    Node** table;                                        // Массив указателей на цепочки
    int size;                                            // Размер таблицы
    int count;                                           // Количество элементов в таблице

    // Хеш-функция, использующая hashU
    int hashFunction(const char* key) {
        return hashU((char*)key, size);                    // Используем hashU для вычисления индекса
    }

    // Функция для нахождения следующего простого числа для нового размера таблицы
    int nextPrime(int n) {
        while (true) {
            bool isPrime = true;
            for (int i = 2; i * i <= n; i++) {             // Проверка на простоту
                if (n % i == 0) {
                    isPrime = false;
                    break;
                }
            }
            if (isPrime) return n;                         // Если простое — возвращаем
            n++;
        }
    }

    // Функция для вычисления средней длины цепочек в таблице
    double calculateAverageChainLength() {
        return (double)count / size;                       // Среднее количество элементов на одну ячейку
    }

    // Функция для рехеширования таблицы
    void rehash() {
        int newSize = nextPrime(size * 2);                 // Новый размер таблицы
        cout << "Рехеширование: новый размер таблицы " << newSize << endl;
        Node** newTable = new Node * [newSize]();          // Новый массив для таблицы

        // Перенос всех элементов в новую таблицу
        for (int i = 0; i < size; i++) {
            Node* current = table[i];
            while (current) {
                int newIndex = hashU(current->key, newSize);   // Используем новую хеш-функцию
                cout << "Перемещение записи: " << current->key << " в новую ячейку " << newIndex << endl;
                Node* nextNode = current->next;
                current->next = newTable[newIndex];            // Перемещаем элемент в новую таблицу
                newTable[newIndex] = current;
                current = nextNode;
            }
        }

        delete[] table;                                        // Освобождаем память старой таблицы
        table = newTable;                                      // Перенаправляем на новую таблицу
        size = newSize;                                        // Обновляем размер таблицы
    }

public:
    HashTable(int initialSize = INITIAL_SIZE) : size(initialSize), count(0) {
        table = new Node * [size]();                           // Инициализация массива указателей на цепочки
    }

    // Деструктор для освобождения памяти
    ~HashTable() {
        for (int i = 0; i < size; i++) {
            Node* current = table[i];
            while (current) {
                Node* temp = current;
                current = current->next;
                delete temp;                               // Удаляем каждый элемент в цепочке
            }
        }
        delete[] table;                                    // Освобождаем память таблицы
    }

    // Функция для проверки, является ли строка числом
    bool isNumber(const char* str) {
        if (str == nullptr || str[0] == '\0') return false;     // Пустая строка не является числом

        for (int i = 0; str[i] != '\0'; ++i) {
            if (!isdigit(str[i])) {                             // Если хотя бы один символ не цифра, возвращаем false
                return false;
            }
        }
        return true;
    }

    void insert(const char* key) {
        if (!isNumber(key)) {                                    // Проверка, что строка представляет собой число
            cout << "Ошибка: строка не является числом!" << endl;
            return;                                              // Прерываем выполнение, если строка не является числом
        }

        if (calculateAverageChainLength() > MAX_CHAIN_LENGTH) {  // Если цепочки слишком длинные, рехешируем
            rehash();
        }
        int index = hashFunction(key);                           // Находим индекс для ключа
        Node* newNode = new Node(key);                           // Создаём новый узел
        newNode->next = table[index];                            // Добавляем его в начало цепочки
        table[index] = newNode;
        count++;                                                 // Увеличиваем счётчик элементов
        cout << "Добавлено: " << key << " в ячейку " << index << endl;
    }


    // Функция поиска по ключу
    Node* search(const char* key) {
        int index = hashFunction(key);                          // Находим индекс для ключа
        Node* current = table[index];                           // Получаем первый элемент цепочки
        int chainIndex = 0;                                     // Индекс элемента в цепочке

        while (current) {
            if (stringsEqual(current->key, key)) {              // Если нашли элемент
                cout << "Номер найден в строке таблицы: " << index << ", в цепочке на позиции: " << chainIndex << endl;
                return current;                                 // Возвращаем найденный узел
            }
            current = current->next;                            // Иначе продолжаем искать в цепочке
            chainIndex++;                                       // Увеличиваем индекс элемента в цепочке
        }

        cout << "Запись не найдена." << endl;                   // Если не нашли, выводим сообщение
        return nullptr;                                         // Возвращаем nullptr, если запись не найдена
    }

    // Функция для удаления элемента по ключу
    void remove(const char* key) {
        int index = hashFunction(key);                          // Находим индекс для ключа
        Node* current = table[index];
        Node* prev = nullptr;
        while (current) {
            if (stringsEqual(current->key, key)) {              // Если нашли, удаляем элемент
                if (prev) {
                    prev->next = current->next;                 // Пропускаем текущий элемент
                }
                else {
                    table[index] = current->next;               // Если элемент первый, обновляем голову цепочки
                }
                delete current;                                 // Удаляем узел
                count--;                                        // Уменьшаем счётчик элементов
                cout << "Удалено: " << key << " из ячейки " << index << endl;
                return;
            }
            prev = current;
            current = current->next;                            // Иначе продолжаем искать
        }
    }

    // Функция для вывода содержимого всей таблицы
    void print() {
        for (int i = 0; i < size; i++) {
            cout << i << ": ";                                  // Выводим индекс ячейки
            Node* current = table[i];
            while (current) {
                cout << "[" << current->key << "] -> ";         // Выводим элементы цепочки
                current = current->next;
            }
            cout << "NULL" << endl;                             // Завершаем цепочку
        }
        cout << "Средняя длина цепочек: " << calculateAverageChainLength() << endl; // Выводим среднюю длину цепочек
    }

    // Функция для загрузки данных из файла
    void loadFromFile(const char* filename) {
        ifstream file(filename);                                // Открываем файл
        if (!file) {
            cout << "Ошибка открытия файла!" << endl;           // Ошибка при открытии
            return;
        }
        char key[20];
        while (file.getline(key, 20)) {                         // Читаем строки из файла
            insert(key);                                        // Добавляем в таблицу
        }
        file.close();                                           // Закрываем файл
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    HashTable phoneBook;
    int choice;
    char key[20];
    char filename[100];

    do {
        cout << "\nМеню:\n";
        cout << "1. Загрузить из файла\n";
        cout << "2. Добавить запись\n";
        cout << "3. Найти запись\n";
        cout << "4. Удалить запись\n";
        cout << "5. Вывести всю таблицу\n";
        cout << "0. Выход\n";
        cout << "Выберите действие: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            cout << "Введите имя файла: ";
            cin.getline(filename, 100);
            phoneBook.loadFromFile(filename);
            break;
        case 2:
            cout << "Введите ключ (номер): ";
            cin.getline(key, 20);
            phoneBook.insert(key);
            break;
        case 3:
            cout << "Введите ключ для поиска: ";
            cin.getline(key, 20);
            if (Node* found = phoneBook.search(key)) {
                cout << "Найдено: " << found->key << endl;
            }
            else {
                cout << "Не найдено." << endl;
            }
            break;
        case 4:
            cout << "Введите ключ для удаления: ";
            cin.getline(key, 20);
            phoneBook.remove(key);
            break;
        case 5:
            phoneBook.print();
            break;
        case 0:
            cout << "Выход..." << endl;
            break;
        default:
            cout << "Неверный ввод." << endl;
        }
    } while (choice != 0);

    return 0;
}
