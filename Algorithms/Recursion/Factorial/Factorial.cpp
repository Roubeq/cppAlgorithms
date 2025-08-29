#include <iostream>

using namespace std;

int factorial(int n) {
    if (n == 1) {
        return 1;
    }
    else {
        return n * factorial(n - 1);
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    int n;
    cout << "Введите число, для которого хотите вычислить факториал: ";
    cin >> n;
    cout << factorial(n);
}

/*
* Этапы формирования рекурсивной функции.
* 1. Разбить задачу на более мелкие подзадачи.
* 2. Написать базовый случай рекурсии(для выхода из неё).
*/