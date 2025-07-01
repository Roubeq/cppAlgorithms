#include "../SSSMatrix.h"
#include <cassert>
#include <fstream>

void createTestFile(const char* filename, const char* content) {
    std::ofstream file(filename);
    file << content;
    file.close();
}

int main() {
    createTestFile("test_matrix.txt", "3 3\n1 0 2\n0 3 0\n4 0 5");  // Создаем тестовый файл

    SSSMatrix matrix;
    matrix.inputFromFileAndPackageMatrix("test_matrix.txt");

    assert(matrix.sumElements() == 15);                             // Проверяем сумму (AD:1+3+5, AU:2, AL:4 ? 15)

    createTestFile("invalid.txt", "3 3\n1 a 2\n0 3 0\n4 0 5");      // Тест на некорректный файл
    SSSMatrix invalidMatrix;
    bool hasError = false;
    try {
        invalidMatrix.inputFromFileAndPackageMatrix("invalid.txt");
    }
    catch (...) {
        hasError = true;
    }
    assert(hasError);

    return 0;
}