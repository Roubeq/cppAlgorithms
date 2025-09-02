#include <iostream>

using namespace std;


// Это хвостовая рекурсия - вызов её в конце, тем самым не нужно сохранять состояние
void fun(int n) {
	if (n == 0) {
		return;
	}
	else {
		cout << n << " ";
	}
	return fun(n - 1);
}

// Это не хвостовая рекурсия, ей как раз таки необходимо проверять что за ней идёт
void funNonTail(int n) {
	if (n == 0) {
		return;
	}
	funNonTail(n - 1);
	cout << n << " ";
}

int main() {
	setlocale(LC_ALL, "Russian");
	fun(3);
	cout << endl;
	funNonTail(3);
	return 0;
}
