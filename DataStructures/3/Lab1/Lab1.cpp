#include <iostream>
#include <fstream>

using namespace std;

struct Node {               // Узел очереди
    int value;
    Node* next;
};

class Queue {               // Очередь на односвязном списке
public:
    Node* first;
    Node* last;
    int size;
public:
    Queue() : first(nullptr), last(nullptr), size(0) {}

    Queue(const Queue& other) : first(nullptr), last(nullptr), size(0) {               // Конструктор копирования
        Node* current = other.first;
        while (current) {
            addToQueue(current->value);
            current = current->next;
        }
    }

    ~Queue() { clear(); }                                                              // Деструктор

    Node* getFirst() { return first; }

    void addToQueue(int value) {                                                       // Добавление элемента в очередь
        Node* newNode = new Node{ value, nullptr };
        if (last) {
            last->next = newNode;
        }
        else {
            first = newNode;
        }
        last = newNode;
        size++;
    }

    int removeFromQueue() {                                       // Удаление элемента из очереди
        if (isEmpty()) throw runtime_error("Очередь пуста!");
        int value = first->value;
        Node* temp = first;
        first = first->next;
        if (!first) last = nullptr;
        delete temp;
        size--;
        return value;
    }

    bool isEmpty() { return size == 0; }                           // Проверка на пустоту

    int getSize() { return size; }                                 // Получение размера очереди

    void printQueue() const {                                      // Вывод очереди в консоль
        Queue copy = *this;                                        // Создаём копию очереди
        while (!copy.isEmpty()) {
            cout << copy.removeFromQueue() << " ";                 // Выводим и удаляем из копии
        }
        cout << endl;
    }

    void clear() {                                                // Очистка всей очереди
        while (!isEmpty()) {
            removeFromQueue();
        }
    }
};

void sortQueue(Queue& q) {                                         // Сортировка очереди
    if (q.isEmpty() || q.getSize() == 1) return;

    for (Node* i = q.getFirst(); i != nullptr; i = i->next) {      // Перебираем все элементы в исходной очереди
        Node* minNode = i;
        Node* current = i->next;

        while (current != nullptr) {
            if (current->value < minNode->value) {
                minNode = current;
            }
            current = current->next;
        }

        if (minNode != i) {
            swap(i->value, minNode->value);
        }
    }
}

Queue mergeQueues(Queue& q1, Queue& q2) {                          // Слияние двух очередей
    Queue mergedQueue;

    Queue copy1 = q1, copy2 = q2;

    while (!copy1.isEmpty() || !copy2.isEmpty()) {
        if (copy1.isEmpty()) {
            mergedQueue.addToQueue(copy2.removeFromQueue());
        }
        else if (copy2.isEmpty()) {
            mergedQueue.addToQueue(copy1.removeFromQueue());
        }
        else {
            if (copy1.getFirst()->value < copy2.getFirst()->value) {
                mergedQueue.addToQueue(copy1.removeFromQueue());
            }
            else {
                mergedQueue.addToQueue(copy2.removeFromQueue());
            }
        }
    }

    return mergedQueue;
}

int getValidatedNumber(const char* prompt, bool allowZero = false) {
    int value;
    std::cout << prompt;
    while (!(std::cin >> value) || value < (allowZero ? 0 : 1)) {
        std::cout << "Ошибка! Введите число " << (allowZero ? ">= 0: " : "> 0: ");
        std::cin.clear();
        while (std::cin.get() != '\n');  // Очистка ввода
    }
    return value;
}

void readFromFile(Queue& q1, Queue& q2) {                          // Функция для чтения двух очередей из файла, разделённых символом '|'
    const char* filename = "data.txt";

    ifstream file(filename);
    if (!file) {
        cout << "Ошибка открытия файла!" << endl;
        return;
    }

    char ch;
    int value;
    bool readingFirstQueue = true;

    while (file >> ws && file.get(ch)) {                           // Читаем символ, пропуская пробелы и переносы строк
        if (ch == '|') {
            readingFirstQueue = false;
        }
        else if (isdigit(ch) || ch == '-' || ch == '+') {          // Проверяем, является ли символ цифрой или знаком числа
            file.putback(ch);                                      // Возвращаем символ в поток, чтобы считать полное число
            if (file >> value) {                                   // Проверяем, успешно ли считалось число
                if (readingFirstQueue) {
                    q1.addToQueue(value);
                }
                else {
                    q2.addToQueue(value);
                }
            }
            else {
                file.clear();                                      // Сбрасываем флаг ошибки
                file.ignore(100, ' ');                             // Пропускаем некорректный ввод до следующего пробела
            }
        }
        else {                                                     // Если символ не является числом
            file.ignore(100, ' ');                                 // Пропускаем текущую строку (слово)
            cout << "Ошибка: найден некорректный ввод в файле. Введите корректное число: ";

            while (!(cin >> value)) {                              // Запрашиваем число у пользователя
                cout << "Некорректный ввод! Попробуйте снова: ";
                cin.clear();                                       // Очищаем ошибку потока
                cin.ignore(10000, '\n');                           // Игнорируем неверный ввод
            }

            if (readingFirstQueue) {
                q1.addToQueue(value);
            }
            else {
                q2.addToQueue(value);
            }
        }
    }

    file.close();
    cout << "Данные загружены из файла!" << endl;
}

int main() {
    setlocale(LC_ALL, "Russian");

    Queue q1, q2;
    int choice;

    cout << "Как вы хотите ввести данные?\n";
    cout << "1. Ввод с клавиатуры\n";
    cout << "2. Ввод из файла\n";
    cout << "Ваш выбор: ";
    cin >> choice;

    if (choice == 1) {
        int n1 = getValidatedNumber("Введите количество элементов в первой очереди: ");
        cout << "Введите элементы первой очереди: ";
        for (int i = 0; i < n1; i++) {
            q1.addToQueue(getValidatedNumber(""));
        }

        int n2 = getValidatedNumber("Введите количество элементов во второй очереди: ");
        cout << "Введите элементы второй очереди: ";
        for (int i = 0; i < n2; i++) {
            q2.addToQueue(getValidatedNumber(""));
        }
    }
    else if (choice == 2) {
        cout << "Данные будут загружены из файла для обеих очередей!" << endl;
        readFromFile(q1,q2);
    }
    else {
        cout << "Неверный выбор!" << endl;
        return 0;
    }

    sortQueue(q1);
    sortQueue(q2);

    cout << "Очередь 1: "; q1.printQueue();
    cout << "Очередь 2: "; q2.printQueue();

    Queue merged = mergeQueues(q1, q2);
    cout << "Объединенная и отсортированная очередь: ";
    merged.printQueue();

    return 0;
}