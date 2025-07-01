#include <iostream>
#include <fstream>

using namespace std;

const int HEAD_VALUE = 1; 
const int START_POSITION = 1;

struct Node {                         // Узел списка
    int value;
    Node* next;
};

class ForwardList {                   // Односвязный циклический список
private:
    Node* head;
    int size;
public:

    ForwardList(int numberOfPeople) : size(numberOfPeople) {           // Конструктор
        head = new Node{ HEAD_VALUE, nullptr };
        Node* current = head;
        for (int i = 2; i <= numberOfPeople; ++i) {
            current->next = new Node{ i, nullptr }; 
            current = current->next;
        }
        current->next = head;                                          // Зацикливаем список
    }

    ~ForwardList() {
        if (!head) return; 

        Node* current = head->next;
        while (current != head) {                                      // Проход по списку и удаление узлов
            Node* temp = current;
            current = current->next;
            delete temp;
        }
        delete head;                                                   // Удаление последнего узла
    }

    void printList() {                                            
        if (!head) return; 

        Node* current = head;
        do {
            cout << current->value << " ";                             // Вывод значения узла
            current = current->next;
        } while (current != head);                                     // Пока не вернулись к началу

        cout << endl;
    }

    int getPosition(int numberOfChoice, int startPosition = 1) {       // Поиск оставшегося человека
        Node* current = head;
        Node* prev = nullptr;
        for (int i = 1; i < startPosition; i++) {                      // Переход к стартовой позиции
            current = current->next;
        }

        while (size > 1) {
            for (int i = 1; i < numberOfChoice; ++i) {                 // Подсчёт до M
                prev = current;
                current = current->next;
            }
            prev->next = current->next;                                // Исключаем узел из списка
            delete current;
            current = prev->next;
            --size;
        }

        int lastHuman = current->value;                                // Последний оставшийся человек
        delete current;
        head = nullptr;                                                // Обнуляем начало
        return lastHuman;
    }
};

int findStartingPerson(int numberOfPeople, int numberOfChoice, int lastHuman) {    // Функция поиска стартового человека
    for (int start = 1; start <= numberOfPeople; ++start) {                        // Проверяем каждую стартовую позицию
        ForwardList listPeople(numberOfPeople);
        if (listPeople.getPosition(numberOfChoice, start) == lastHuman) {
            return start;
        }
    }
    return -1; 
}


int getValidatedInput(const char* prompt, int minValue, int maxValue) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;

        if (std::cin.fail() || value < minValue || value > maxValue) {
            std::cout << "Ошибка! Введите число в диапазоне " << minValue << " - " << maxValue << std::endl;
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
        else {
            std::cin.ignore(10000, '\n');
            return value;
        }
    }
}

int getValidatedInputFromFile(ifstream& file, int minValue, int maxValue) {
    string token;
    int value;
    while (true) {
        if (!(file >> token)) {
            cout << "Ошибка! Файл содержит недостаточно данных." << endl;
            exit(1);
        }

        bool isNumber = true;
        value = 0;  // Обнуляем перед конвертацией

        for (char c : token) {
            if (c < '0' || c > '9') {  // Проверяем, состоит ли строка из цифр
                isNumber = false;
                break;
            }
            value = value * 10 + (c - '0');  // Преобразуем строку в число
        }

        if (!isNumber) {
            cout << "Ошибка! '" << token << "' не является числом. Исправьте файл." << endl;
            exit(1);
        }

        if (value < minValue || value > maxValue) {
            cout << "Ошибка! Число из файла выходит за пределы диапазона " << minValue << " - " << maxValue << ". Исправьте файл." << endl;
            exit(1);
        }

        return value;
    }
}


int main() {
    setlocale(LC_ALL, "Russian");

    int numberOfPeople, numberOfChoice, lastHuman;
    char inputMethod;

    cout << "Выберите способ ввода данных:\n";
    cout << "1. Ввод с клавиатуры\n";
    cout << "2. Ввод из файла (input.txt)\n";
    cout << "Ваш выбор: ";
    cin >> inputMethod;
    cin.ignore(10000, '\n');

    if (inputMethod == '1') {
        numberOfPeople = getValidatedInput("Введите N (количество людей): ", 1, 10000);
        numberOfChoice = getValidatedInput("Введите M (число счета): ", 1, 10000);
        lastHuman = getValidatedInput("Введите L (номер оставшегося человека): ", 1, numberOfPeople);
    }
    else if (inputMethod == '2') {
        ifstream inputFile("input.txt");
        if (!inputFile) {
            cerr << "Ошибка! Не удалось открыть файл input.txt" << endl;
            return 1;
        }

        numberOfPeople = getValidatedInputFromFile(inputFile, 1, 10000);
        numberOfChoice = getValidatedInputFromFile(inputFile, 1, 10000);
        lastHuman = getValidatedInputFromFile(inputFile, 1, numberOfPeople);

        inputFile.close();
    }
    else {
        cout << "Ошибка! Неверный выбор." << endl;
        return 1;
    }

    int lastPerson;
    ForwardList circle(numberOfPeople);
    if (numberOfChoice == 1) {
        lastPerson = numberOfPeople;
    }
    else {
        lastPerson = circle.getPosition(numberOfChoice);
    }

    cout << "Оставшийся человек: " << lastPerson << "\n";

    int startPerson;
    if (numberOfChoice == 1) {
        startPerson = (lastHuman % numberOfPeople) + 1;
    }
    else {
        startPerson = findStartingPerson(numberOfPeople, numberOfChoice, lastHuman);
    }
    cout << "Начинать счет нужно с: " << startPerson << "\n";

    return 0;
}