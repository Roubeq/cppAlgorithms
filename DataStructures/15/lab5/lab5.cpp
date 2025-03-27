#include <iostream>
#include <fstream>
#include <Windows.h>

using namespace std;

const int INITIAL_SIZE = 10;                            // Начальный размер хеш-таблицы
const int MAX_CHAIN_LENGTH = 3;                         // Максимальная средняя длина цепочки перед рехешированием

struct Node {
    long long key;                                      // Номер телефона
    char name[70];                                      // Имя контакта
    Node* next;                                         // Указатель на следующий элемент в цепочке

    Node(long long k, const char* n) : key(k), next(nullptr) {
        int i = 0;
        while (n[i] != '\0' && i < 69) {
            name[i] = n[i];
            i++;
        }
        name[i] = '\0';                                 // Гарантируем завершение строки
    }
};

class HashTable {
private:
    Node** table;                                       // Массив указателей на списки (цепочки)
    int size;                                           // Текущий размер таблицы
    int count;                                          // Количество элементов в таблице

    int hashFunction(long long key) {
        return key % size;                              // Простая хеш-функция на основе остатка от деления
    }

    int nextPrime(int n) {                              // Поиск следующего простого числа
        while (true) {
            bool isPrime = true;
            for (int i = 2; i * i <= n; i++) {
                if (n % i == 0) {
                    isPrime = false;
                    break;
                }
            }
            if (isPrime) return n;
            n++;
        }
    }

    double calculateAverageChainLength() {              // Вычисляет среднюю длину цепочек
        return (double)count / size;
    }

    void rehash() {                                     // Пересоздание таблицы при превышении длины цепочек
        int newSize = nextPrime(size * 2);
        cout << "Рехеширование: новый размер таблицы " << newSize << endl;
        Node** newTable = new Node * [newSize]();

        for (int i = 0; i < size; i++) {
            Node* current = table[i];
            while (current) {
                int newIndex = current->key % newSize;
                cout << "Перемещение записи: " << current->key << " - " << current->name << " в новую ячейку " << newIndex << endl;
                Node* nextNode = current->next;
                current->next = newTable[newIndex];     // Перенос в новую таблицу
                newTable[newIndex] = current;
                current = nextNode;
            }
        }

        delete[] table;                                 // Освобождение памяти старой таблицы
        table = newTable;
        size = newSize;
    }

public:
    HashTable(int initialSize = INITIAL_SIZE) : size(initialSize), count(0) {
        table = new Node * [size]();                    // Инициализация массива указателей
    }

    ~HashTable() {                                      // Деструктор для очистки памяти
        for (int i = 0; i < size; i++) {
            Node* current = table[i];
            while (current) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
        }
        delete[] table;
    }

    void insert(long long key, const char* name) {
        if (calculateAverageChainLength() > MAX_CHAIN_LENGTH) {
            rehash();                                   // Увеличение таблицы при достижении лимита средней длины цепочек
        }
        int index = hashFunction(key);
        Node* newNode = new Node(key, name);
        newNode->next = table[index];                   // Добавление в начало списка
        table[index] = newNode;
        count++;
        cout << "Запись: " << key << " - " << name << " добавлена в ячейку " << index << endl;
    }

    Node* search(long long key) {
        int index = hashFunction(key);
        Node* current = table[index];
        while (current) {
            if (current->key == key) {
                return current;                         // Найденный узел возвращается
            }
            current = current->next;
        }
        return nullptr;                                 // Если не найдено
    }

    void remove(long long key) {
        int index = hashFunction(key);
        Node* current = table[index];
        Node* prev = nullptr;
        while (current) {
            if (current->key == key) {
                if (prev) {
                    prev->next = current->next;         // Удаление узла из списка
                }
                else {
                    table[index] = current->next;       // Обновление головы списка
                }
                delete current;
                count--;
                cout << "Запись с номером " << key << " удалена из ячейки " << index << endl;
                return;
            }
            prev = current;
            current = current->next;
        }
    }

    void print() {                                      // Вывод всей таблицы с цепочками
        for (int i = 0; i < size; i++) {
            cout << i << ": ";
            Node* current = table[i];
            while (current) {
                cout << "[" << current->key << " - " << current->name << "] -> ";
                current = current->next;
            }
            cout << "NULL" << endl;
        }
        cout << "Средняя длина цепочек: " << calculateAverageChainLength() << endl;
    }

    void loadFromFile(const char* filename) {           // Чтение данных из файла
        ifstream file(filename);
        if (!file) {
            cout << "Ошибка открытия файла!" << endl;
            return;
        }
        long long key;
        char name[70];
        while (file >> key) {
            file.ignore();
            file.getline(name, 70);
            insert(key, name);
        }
        file.close();
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    HashTable phoneBook;
    int choice;
    long long key;
    char name[70];
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

        switch (choice) {
        case 1:
            cout << "Введите имя файла: ";
            cin >> filename;
            phoneBook.loadFromFile(filename);
            cout << "Данные загружены." << endl;
            break;
        case 2:
            cout << "Введите номер телефона: ";
            cin >> key;
            cin.ignore();
            cout << "Введите имя: ";
            cin.getline(name, 70);
            phoneBook.insert(key, name);
            break;
        case 3:
            cout << "Введите номер телефона для поиска: ";
            cin >> key;
            if (Node* found = phoneBook.search(key)) {
                cout << "Найдено: " << found->name << endl;
            }
            else {
                cout << "Запись не найдена." << endl;
            }
            break;
        case 4:
            cout << "Введите номер телефона для удаления: ";
            cin >> key;
            phoneBook.remove(key);
            break;
        case 5:
            phoneBook.print();
            break;
        case 0:
            cout << "Выход..." << endl;
            break;
        default:
            cout << "Некорректный ввод. Попробуйте снова." << endl;
        }
    } while (choice != 0);

    return 0;
}
