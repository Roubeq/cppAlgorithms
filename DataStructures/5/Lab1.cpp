#include <iostream>
#include <fstream>

struct ListNode {
    int value;       // Значение узла
    ListNode* next;  // Указатель на следующий элемент

    ListNode(int val) : value(val), next(nullptr) {}       // Конструктор узла
};

class SimpleLinkedList {
private:
    ListNode* firstElement;                                // Первый элемент списка

public:
    SimpleLinkedList() : firstElement(nullptr) {}          // Конструктор списка

    void addToEnd(int val) {                               // Добавление элемента в конец списка
        ListNode* newNode = new ListNode(val);             // Создаем новый узел
        if (!firstElement) {                               // Если список пуст
            firstElement = newNode;
            return;
        }
        ListNode* current = firstElement;
        while (current->next) {                            // Идем до конца списка
            current = current->next;
        }
        current->next = newNode;                           // Добавляем новый элемент
    }

    void swapFirstAndLast() {
        if (!firstElement || !firstElement->next) {
            std::cout << "Мало элементов для замены!" << std::endl;
            return;
        }

        ListNode* prevLast = nullptr;
        ListNode* last = firstElement;
        ListNode* second = firstElement->next;

        while (last->next) {
            prevLast = last;
            last = last->next;
        }

        if (prevLast) {
            prevLast->next = firstElement;
        }

        last->next = second;
        firstElement->next = nullptr;
        firstElement = last;
    }

    void printAll() {                                      // Функция для вывода списка
        ListNode* current = firstElement;
        while (current) {
            std::cout << current->value << " -> ";
            current = current->next;
        }
        std::cout << "NULL" << std::endl;
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    SimpleLinkedList myList;

    int choice;
    std::cout << "Выберите способ ввода данных:\n";
    std::cout << "1. Ввод из файла\n";
    std::cout << "2. Ввод вручную\n";
    std::cout << "Ваш выбор: ";
    std::cin >> choice;

    switch (choice) {
    case 1: {                                             // Чтение данных из файла
        std::ifstream inputFile("input.txt");             // Открываем файл для чтения
        if (!inputFile) {                                 // Проверка на успешное открытие
            std::cerr << "Ошибка открытия файла!" << std::endl;
            return 1;
        }

        char token[20];                                   // Буфер для хранения числа
        while (inputFile >> token) {                      // Читаем данные из файла
            bool isNumber = true;
            for (int i = 0; token[i] != '\0'; i++) {      // Проверяем, состоит ли строка только из цифр
                if (token[i] < '0' || token[i] > '9') {
                    isNumber = false;
                    break;
                }
            }

            if (isNumber) {                               // Если строка содержит только цифры
                int number = 0;
                for (int i = 0; token[i] != '\0'; i++) {
                    number = number * 10 + (token[i] - '0');  // Преобразуем строку в число вручную
                }
                myList.addToEnd(number);                  // Добавляем число в список
            }
            else {                                        // Если найден нечисловой ввод
                int correctNumber;
                std::cerr << "Ошибка: '" << token << "' не является числом. Введите корректное число: ";
                while (!(std::cin >> correctNumber)) {    // Запрашиваем число у пользователя
                    std::cerr << "Некорректный ввод! Попробуйте снова: ";
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                }
                myList.addToEnd(correctNumber);           // Добавляем число в список
            }
        }
        inputFile.close();                                // Закрываем файл
        break;
    }

    case 2: {                                             // Ввод данных вручную
        int count;
        std::cout << "Введите количество чисел: ";
        while (!(std::cin >> count) || count <= 0) {      // Проверка корректности ввода
            std::cerr << "Некорректный ввод! Введите положительное число: ";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }

        std::cout << "Введите " << count << " чисел:\n";
        for (int i = 0; i < count; i++) {               // Считываем введенные числа
            int number;
            while (!(std::cin >> number)) {             // Проверка корректности ввода числа
                std::cerr << "Некорректный ввод! Введите число: ";
                std::cin.clear();
                std::cin.ignore(10000, '\n');
            }
            myList.addToEnd(number);                    // Добавляем число в список
        }
        break;
    }

    default:                                            // Ошибка выбора
        std::cerr << "Ошибка: Неверный выбор!" << std::endl;
        return 1;
    }

    std::cout << "Список до замены: ";
    myList.printAll();                                  // Вывод списка до замены

    myList.swapFirstAndLast();                         // Замена первого и последнего элемента

    std::cout << "Список после замены: ";
    myList.printAll();                                  // Вывод списка после замены

    return 0;
}