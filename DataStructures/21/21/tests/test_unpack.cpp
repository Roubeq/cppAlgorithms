#include "../SparseMatrix.h"
#include <cassert>
#include <sstream>
#include <iostream>

void testUnpackValid() {
    SparseMatrix matrix;
    int A[] = { 1, 2 };
    int LI[] = { 0, 1 };
    int LJ[] = { 0, 1 };
    matrix.fillManual(2, 2, 2, A, LI, LJ);

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    matrix.unpackAndPrint();

    std::cout.rdbuf(old);

    std::string output = buffer.str();
    assert(output.find("1 0") != std::string::npos);
    assert(output.find("0 2") != std::string::npos);
}

void testUnpackEmpty() {
    SparseMatrix matrix;

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    matrix.unpackAndPrint(); // Пустая матрица — просто ничего не напечатает

    std::cout.rdbuf(old);

    std::string output = buffer.str();
    assert(output.empty());  // Ничего не напечатано
}

void testSingleNonZeroElement() {
    SparseMatrix matrix;
    int A[] = { 5 };         // Один ненулевой элемент
    int LI[] = { 0 };        // Индекс строки
    int LJ[] = { 1 };        // Индекс столбца

    matrix.fillManual(2, 2, 1, A, LI, LJ);         // Матрица 2x2, 1 ненулевой элемент

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    matrix.unpackAndPrint();                       // Ожидаем, что печататься будет только 5 в позиции (0, 1)

    std::cout.rdbuf(old);

    std::string output = buffer.str();
    assert(output.find("5") != std::string::npos);
    assert(output.find("1") == std::string::npos); // Строки с нулями не должны выводиться
}

int main() {
    testUnpackValid();
    testUnpackEmpty();
    testSingleNonZeroElement();
    return 0;
}
