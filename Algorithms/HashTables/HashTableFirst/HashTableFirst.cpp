#include <iostream>
#include <windows.h>

using namespace std;
const double goldenRatio = 0.6180339887;
class HashTable {
    struct Entry {
        int key;
        int value;
        bool isActive;
        Entry() : key(0), value(0), isActive(false) {}
        Entry(int k, int v) : key(k), value(v), isActive(true) {}
    };

    void rehash() {
        int oldSize = sizeTable;
        sizeTable *= 3;
        Entry** oldTable = table;
        table = new Entry * [sizeTable]();
        for (int i = 0; i < sizeTable; ++i) {
            table[i] = new Entry();
        }
        countElements = 0;

        cout << "REHASH to new size: " << sizeTable << endl;

        for (int i = 0; i < oldSize; ++i) {
            if (oldTable[i]->isActive) {
                insert(oldTable[i]->key, oldTable[i]->value);
            }
            delete oldTable[i];
        }
        delete[] oldTable;
    }
public:
    HashTable(int initialSize = 11) : sizeTable(initialSize), countElements(0) {
        table = new Entry * [sizeTable]();
        for (int i = 0; i < sizeTable; ++i) {
            table[i] = new Entry();
        }
    }

    ~HashTable() {
        for (int i = 0; i < sizeTable; ++i) {
            delete table[i];
        }
        delete[] table;
    }
    Entry** table;
    int sizeTable;
    int countElements;
    const double maxOverload = 0.7;

    int hashMultiplication(int& key) {
        double result = key * goldenRatio - floor(key * goldenRatio);
        return int(sizeTable * result);
    }

    int hashDivision(int& key) {
        return key % sizeTable;
    }

    int hashMidSquare(int& key) {
        key *= key;
        key = (key >> 8) & 0xFFFF;
        return key % sizeTable;
    }

    int hashFold(int& key) {
        int sum = 0;
        while (key > 0) {
            sum += key % 1000;
            key /= 1000;
        }
        return sum % sizeTable;
    }

    int hashStringSum(const string& key) {
        int sum = 0;
        for (char c : key) sum += c;
        return sum % sizeTable;
    }

    int hashPolynomial(const string& key) {
        const int p = 31;
        int hash = 0;
        for (char c : key) {
            hash = (hash * p + c) % sizeTable;
        }
        return hash;
    }

    void insert(int key, int value) {
        if (countElements >= sizeTable * maxOverload) {
            rehash();
        }
        int index = hashMultiplication(key);
        int i = 0;

        while (table[(index + i * i) % sizeTable]->isActive) {
            if (table[(index + i * i) % sizeTable]->key == key) {
                table[(index + i * i) % sizeTable]->value = value;
                return;
            }
            i++;
        }
        int pos = (index + i * i) % sizeTable;
        table[pos]->key = key;
        cout << "Число добавлено в позицию" << pos << endl;
        table[pos]->value = value;
        table[pos]->isActive = true;
        countElements++;
    }

    void remove(int keyForRemove) {
        int index = hashMultiplication(keyForRemove);
        int i = 0;

        while (table[(index + i * i) % sizeTable]->isActive) {
            int pos = (index + i * i) % sizeTable;
            if (table[pos]->key == keyForRemove) {
                table[pos]->key = 0;
                table[pos]->isActive = false;
                table[pos]->value = 0;
                countElements--;
                cout << "Ключ " << keyForRemove << " удалён" << endl;
                return;
            }
            i++;
        }
    }

    void print() {
        for (int i = 0; i < sizeTable; i++) {
            if (table[i]->key != 0) {
                cout << "Key - " << "[ " << table[i]->key << " ]" << " -> " << table[i]->value << endl;
            }
            else cout << i << " pos" << " is empty" << endl;
        }
    }

    void search(int searchKey) {
        int index = hashMultiplication(searchKey);
        int i = 0;

        while (table[(index + i * i) % sizeTable]->isActive) {
            int pos = (index + i * i) % sizeTable;
            if (table[pos]->key == searchKey) {
                cout << "Found!!" << endl;
                cout << "Key - " << "[ " << table[pos]->key << " ]" << " -> " << table[pos]->value << endl;
                return;
            }
            i++;
        }
    }

};

int main() {
    setlocale(LC_ALL, "Russian");
    HashTable ht(5); 

    ht.insert(10, 100);
    ht.insert(20, 200);
    ht.insert(30, 300);
    ht.insert(40, 400);
    ht.insert(50, 500);

    cout << "\nПосле добавления:" << endl;
    ht.print();

    cout << "\nПоиск элементов:" << endl;
    ht.search(20);
    ht.search(99);

    cout << "\nУдаление элемента:" << endl;
    ht.remove(20);
    ht.remove(99);

    cout << "\nПосле удаления:" << endl;
    ht.print();

    return 0;
}