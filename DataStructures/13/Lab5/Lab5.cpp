#include <iostream>
#include <fstream>
#include <windows.h>
using namespace std;

const double goldenRatio = 0.6180339887;        // Золотое сечение для хеш-функции

struct NumberPhone {
    long long phoneNumber;                      // Номер телефона (ключ)
    char value[50];                             // Имя и фамилия

    NumberPhone() : phoneNumber(0) {
        value[0] = '\0';                        // По умолчанию имя пустое
    }
};

class HashTable {
    NumberPhone* table;                         // Хеш-таблица
    int countElements;                          // Количество элементов
    int sizeTable;                              // Размер таблицы

    int hashFunction(long long key) {           // Хеш-функция методом умножения
        double frac = key * goldenRatio - floor(key * goldenRatio);
        return (int)(sizeTable * frac);
    }

    bool isPrime(int n) {
        if (n < 2) return false;
        for (int i = 2; i * i <= n; i++) {
            if (n % i == 0) return false;
        }
        return true;
    }

    int getNextPrime(int n) {
        while (!isPrime(n)) n++;
        return n;
    }
    void rehash() {                             // Перехеширование при заполнении таблицы более чем на 75%(спорно)
        int oldSize = sizeTable;
        sizeTable = getNextPrime(sizeTable * 2);// Увеличиваем таблицу вдвое(так то можно на побольше)
        NumberPhone* oldTable = table;
        table = new NumberPhone[sizeTable];
        countElements = 0;
        cout << "REHASH С " << oldSize << "ДО " << sizeTable << endl;
        for (int i = 0; i < oldSize; i++) {
            if (oldTable[i].phoneNumber != 0) { // Переносим существующие элементы
                insert(oldTable[i].phoneNumber, oldTable[i].value);
            }
        }

        delete[] oldTable;
    }

public:
    HashTable(int size = 11) : sizeTable(size), countElements(0) {
        table = new NumberPhone[size];
    }

    ~HashTable() {
        delete[] table;
    }

    char* search(long long key) {               // Поиск по номеру
        int index = hashFunction(key);
        int i = 0;

        while (table[(index + i * i) % sizeTable].phoneNumber != 0) {  // Квадратичное пробирование
            int pos = (index + i * i) % sizeTable;
            if (table[pos].phoneNumber == key) {                       // Если номер найден, возвращаем имя
                return table[pos].value;
            }
            i++;
        }
        return nullptr;
    }
      
    void remove(long long key) {                  // Удаление номера (зануляем данные)
        int index = hashFunction(key);
        int i = 0;

        while (table[(index + i * i) % sizeTable].phoneNumber != 0) {
            int pos = (index + i * i) % sizeTable;
            if (table[pos].phoneNumber == key) {  // Если нашли, обнуляем
                table[pos].phoneNumber = 0;
                table[pos].value[0] = '\0';
                countElements--;
                return;
            }
            i++;
        }
    }

    void insert(long long key, const char* value) {                     // Вставка нового номера
        if (countElements >= (sizeTable * 3) / 4) {
            rehash();
        }

        int index = hashFunction(key);
        int i = 0;

        while (table[(index + i * i) % sizeTable].phoneNumber != 0) {   // Квадратичное пробирование
            int pos = (index + i * i) % sizeTable;
            if (table[pos].phoneNumber == key) {                        // Если номер уже есть, обновляем ФИО
                int j = 0;
                while (value[j] != '\0' && j < 49) {
                    table[pos].value[j] = value[j];
                    j++;
                }
                table[pos].value[j] = '\0';
                cout << "Номер уже был в телефонной книге. Обновлено значение на " << table[pos].value << endl;
                return;
            }
            i++;
        }

        int pos = (index + i * i) % sizeTable;
        table[pos].phoneNumber = key;               // Вставляем новый номер
        cout << "Вставили номер " << key << " в позицию таблицы " << pos << endl;
        int j = 0;
        while (value[j] != '\0' && j < 49) {
            table[pos].value[j] = value[j];
            j++;
        }
        table[pos].value[j] = '\0';
        countElements++;
    }

    void print() {                                  // Вывод хеш-таблицы
        for (int i = 0; i < sizeTable; i++) {
            if (table[i].phoneNumber != 0) {
                cout << "[" << i << "] " << table[i].phoneNumber << " -> " << table[i].value << endl;
            }
            else {
                cout << "[" << i << "] EMPTY" << endl;
            }
        }
    }
};

void loadFromFile(HashTable& table, const char* filename) {  // Загрузка из файла
    ifstream file(filename);
    if (!file) {
        cout << "Ошибка: Не удалось открыть файл " << filename << endl;
        return;
    }

    char phoneStr[20];
    char value[50];

    while (file >> phoneStr) {                              // Читаем номер как строку
        int len = 0;
        long long phone = 0;

        while (phoneStr[len] != '\0') {                     // Проверяем, что номер состоит только из цифр
            if (phoneStr[len] < '0' || phoneStr[len] > '9') {
                cout << "Ошибка: Некорректный номер " << phoneStr << endl;
                file.ignore(1000, '\n');
                break;
            }
            len++;
        }

        if (len != 11) {                                    // Длина номера должна быть ровно 11 цифр
            cout << "Ошибка: Номер должен содержать ровно 11 цифр -> " << phoneStr << endl;
            file.ignore(1000, '\n');
            continue;
        }

        phone = 0;
        for (int i = 0; i < 11; i++) {                      // Преобразуем строку в число
            phone = phone * 10 + (phoneStr[i] - '0');
        }

        char ch;
        file.get(ch);
        while (ch == ' ' || ch == '\t') {                   // Пропускаем пробелы перед именем
            file.get(ch);
        }

        int i = 0;
        while (ch != '\n' && !file.eof() && i < 49) {       // Читаем имя посимвольно
            value[i++] = ch;
            file.get(ch);
        }
        value[i] = '\0';

        table.insert(phone, value);                         // Вставляем в хеш-таблицу
    }

    file.close();
}

void printMenu() {
    cout << "\n=====================================\n";
    cout << "         ТЕЛЕФОННЫЙ СПРАВОЧНИК     \n";
    cout << "=====================================\n";
    cout << "  1 - Загрузить данные из файла\n";
    cout << "  2 - Добавить номер\n";
    cout << "  3 - Найти номер\n";
    cout << "  4 - Удалить номер\n";
    cout << "  5 - Показать всю таблицу\n";
    cout << "  0 - Выйти из программы\n";
    cout << "=====================================\n";
    cout << "  Выберите действие: ";
}

int main() {
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    HashTable table;
    char filename[] = "phones.txt";

    int choice;
    long long phone;
    char value[50];

    do {
        printMenu();
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Ошибка: введите число от 0 до 5!\n";
            continue;
        }

        cout << "\n-------------------------------------\n";

        switch (choice) {
        case 1:
            loadFromFile(table, filename);
            cout << "Данные успешно загружены из файла!\n";
            break;

        case 2:
            cout << "Введите номер (11 цифр): ";
            cin >> phone;

            if (phone < 10000000000LL || phone > 99999999999LL) {
                cout << "Ошибка: номер должен содержать ровно 11 цифр!\n";
                cin.clear();
                cin.ignore(10000, '\n');
                break;
            }

            cin.ignore();
            cout << "Введите имя: ";
            cin.getline(value, 50);

            table.insert(phone, value);
            cout << "Номер добавлен!\n";
            break;

        case 3:
            cout << "Введите номер для поиска: ";
            cin >> phone;
            if (table.search(phone) == nullptr) {
                cout << "В вашей телефонной книге нет такого номера!" << endl;
            }
            else {
                cout << table.search(phone) << endl;
            }
            break;

        case 4:
            cout << "Введите номер для удаления: ";
            cin >> phone;

            if (table.search(phone) == nullptr) {
                cout << "Ошибка: номер не найден в телефонной книге!\n";
            }
            else {
                table.remove(phone);
                cout << "Номер успешно удалён!\n";
            }
            break;

        case 5:
            cout << "Текущая таблица:\n";
            table.print();
            break;

        case 0:
            cout << "Выход из программы...\n";
            break;

        default:
            cout << "Ошибка: неверный выбор. Попробуйте ещё раз!\n";
        }

        cout << "-------------------------------------\n";

    } while (choice != 0);

    return 0;
}