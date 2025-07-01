#include <iostream>
#include <fstream>
#include <windows.h>

using namespace std;

struct PhoneNumber {
    long long number;
    char nameSurname[70];

    PhoneNumber() : number(0) {
        nameSurname[0] = '\0';
    }
};

class HashTable {
    PhoneNumber* table;
    int sizeTable;
    int countNumbers;

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

    int hashFunction(long long key) {
        int sum = 0;
        while (key > 0) {
            sum += key % 1000;
            key /= 1000;
        }
        return sum % sizeTable;
    }

    void rehash() {
        int oldSize = sizeTable;
        sizeTable = getNextPrime(sizeTable * 2);
        PhoneNumber* oldTable = table;
        table = new PhoneNumber[sizeTable];
        countNumbers = 0;
        cout << "| Проиcходит рехеширование! Размер увеличился с " << oldSize << " до " << sizeTable << "|" << endl;
        for (int i = 0; i < oldSize; i++) {
            if (oldTable[i].number != 0) {
                insert(oldTable[i].number, oldTable[i].nameSurname);
            }
        }
        delete[] oldTable;
    }

public:
    HashTable(int size = 11) : sizeTable(size), countNumbers(0) {
        table = new PhoneNumber[size];
    }

    ~HashTable() {
        delete[] table;
    }

    void insert(long long number, const char* nameSurname) {
        if (countNumbers >= (sizeTable * 3) / 4) {
            rehash();
        }

        int index = hashFunction(number);
        int i = 0;
        cout << "Добавление: " << number << ", начальный хеш: " << index << endl;

        while (table[(index + i * i) % sizeTable].number != 0) {
            if (table[(index + i * i) % sizeTable].number == number) {
                int j = 0;
                while (nameSurname[j] != '\0' && j < 69) {
                    table[(index + i * i) % sizeTable].nameSurname[j] = nameSurname[j];
                    j++;
                }
                table[(index + i * i) % sizeTable].nameSurname[j] = '\0';
                cout << "Номер " << number << " уже существует. Обновлено имя." << endl;
                return;
            }
            i++;
        }

        int pos = (index + i * i) % sizeTable;
        table[pos].number = number;
        int j = 0;
        while (nameSurname[j] != '\0' && j < 69) {
            table[pos].nameSurname[j] = nameSurname[j];
            j++;
        }
        table[pos].nameSurname[j] = '\0';
        table[pos].nameSurname[69] = '\0';
        countNumbers++;

        cout << "Номер " << number << " добавлен в индекс " << pos << endl;
    }

    void search(long long number) {
        int index = hashFunction(number);
        int i = 0;
        while (table[(index + i * i) % sizeTable].number != 0) {
            int pos = (index + i * i) % sizeTable;
            if (table[pos].number == number) {
                cout << "Найден: " << table[pos].nameSurname << endl;
                return;
            }
            i++;
        }
        cout << "Запись не найдена!" << endl;
    }

    void remove(long long number) {
        int index = hashFunction(number);
        int i = 0;
        while (table[(index + i * i) % sizeTable].number != 0) {
            int pos = (index + i * i) % sizeTable;
            if (table[pos].number == number) {
                table[pos].number = 0;
                table[pos].nameSurname[0] = '\0';
                countNumbers--;
                cout << "Запись удалена." << endl;
                return;
            }
            i++;
        }
        cout << "Запись не найдена!" << endl;
    }

    void loadFromFile(const char* filename) {
        ifstream file(filename);
        if (!file) {
            cout << "Ошибка: не удалось открыть файл " << filename << endl;
            return;
        }

        char phoneStr[20];
        char nameSurname[70];

        while (file >> phoneStr) {
            long long phone = atoll(phoneStr);
            file.get();
            file.getline(nameSurname, 70);
            insert(phone, nameSurname);
        }
        file.close();
    }

    void print() {
        for (int i = 0; i < sizeTable; i++) {
            if (table[i].number != 0) {
                cout << "[" << i << "] " << table[i].number << " -> " << table[i].nameSurname << endl;
            }
            else {
                cout << "[" << i << "] EMPTY" << endl;
            }
        }
    }
};

bool isValidNumber(const char* str) {
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] < '0' || str[i] > '9') return false;
        i++;
    }
    return i > 0;
}

long long stringToLongLong(const char* str) {
    long long result = 0;
    int i = 0;
    while (str[i] != '\0') {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    return result;
}

int main() {
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    HashTable table;
    char input[12];
    long long number;
    char name[70];
    char filename[100];

    do {
        cout << "\n======================================\n";
        cout << "                 МЕНЮ                    " << endl;
        cout << "======================================" << endl;
        cout << " 1. Загрузить из файла" << endl;
        cout << " 2. Добавить запись" << endl;
        cout << " 3. Найти запись" << endl;
        cout << " 4. Удалить запись" << endl;
        cout << " 5. Вывести хеш-таблицу" << endl;
        cout << " 0. Выход" << endl;
        cout << "--------------------------------------" << endl;
        cout << "Введите ваш выбор: ";

        cin >> input;
        if (!isValidNumber(input)) {
            cout << "Ошибка! Введите корректное число!" << endl;
            continue;
        }
        int choice = stringToLongLong(input);

        switch (choice) {
        case 1:
            cout << "\nВведите имя файла: ";
            cin >> filename;
            table.loadFromFile(filename);
            break;
        case 2:
            cout << "\nВведите номер телефона: ";
            cin >> input;
            if (!isValidNumber(input)) {
                cout << "Ошибка! Введите корректный номер!" << endl;
                continue;
            }
            number = stringToLongLong(input);
            if (number < 10000000000LL || number > 99999999999LL) {
                cout << "Ошибка! Номер должен быть в диапазоне от 10000000000 до 99999999999." << endl;
                continue;
            }
            cout << "Введите имя и фамилию: ";
            cin.ignore();
            cin.getline(name, 70);
            table.insert(number, name);
            break;
        case 3:
            cout << "\nВведите номер телефона для поиска: ";
            cin >> number;
            if (!isValidNumber(input)) {
                cout << "Ошибка! Введите корректный номер!" << endl;
                continue;
            }
            if (number < 10000000000LL || number > 99999999999LL) {
                cout << "Ошибка! Номер должен быть в диапазоне от 10000000000 до 99999999999." << endl;
                continue;
            }
            table.search(number);
            break;
        case 4:
            cout << "\nВведите номер телефона для удаления: ";
            cin >> number;
            if (!isValidNumber(input)) {
                cout << "Ошибка! Введите корректный номер!" << endl;
                continue;
            }
            if (number < 10000000000LL || number > 99999999999LL) {
                cout << "Ошибка! Номер должен быть в диапазоне от 10000000000 до 99999999999." << endl;
                continue;
            }
            table.remove(number);
            break;
        case 5:
            cout << "\nТекущая хеш-таблица:" << endl;
            table.print();
            break;
        case 0:
            cout << "\nВыход..." << endl;
            return 0;
        default:
            cout << "\nНеверный выбор. Попробуйте снова." << endl;
        }
    } while (true);
}
