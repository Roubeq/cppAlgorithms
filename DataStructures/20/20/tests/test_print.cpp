#include <cassert>
#include <sstream>
#include <cstring>
#include "../SSSMatrix.h"

void createTestFile(const char* filename, const char* content) {
    std::ofstream file(filename);
    file << content;
    file.close();
}

int main() {
    createTestFile("test_matrix.txt", "3 3\n1 0 2\n0 3 0\n4 0 5");
    SSSMatrix matrix;
    matrix.inputFromFileAndPackageMatrix("test_matrix.txt");

    std::stringstream buffer;           // Перенаправляем вывод
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    matrix.printArrays();
    std::cout.rdbuf(old);               // Восстанавливаем стандартный вывод

    std::string output = buffer.str();  // Получаем и проверяем вывод
    assert(output.find("Массив главной диагонали (AD): 1 3 5") != std::string::npos);

    return 0;
}