#include "../SparseMatrix.h"
#include <cassert>
#include <fstream>
#include <sstream>

void testPrintArrays() {
    std::ofstream("print.txt") << "2 2 1 0 0 2";

    SparseMatrix matrix;
    assert(matrix.readAndPack("print.txt") == true);

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    matrix.printArrays();
    std::cout.rdbuf(old);

    std::string output = buffer.str();
    assert(output.find("массив ненулевых элементов (A): 1 2") != std::string::npos);
    assert(output.find("массив строк (LI): 0 1") != std::string::npos);
    assert(output.find("массив столбцов (LJ): 0 1") != std::string::npos);

    std::remove("print.txt");
}

int main() {
    testPrintArrays();
    return 0;
}