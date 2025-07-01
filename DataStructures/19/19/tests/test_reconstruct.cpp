#include "../19.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>

void createTestFile(const char* filename, const char* content) {
    std::ofstream file(filename);
    file << content;
    file.close();
}

void captureOutput(const CCSMatrix& matrix, void (CCSMatrix::* func)() const) {
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    (matrix.*func)();

    std::cout.rdbuf(old);
    buffer.str();
}

void testReconstruction() {
    createTestFile("reconstruct_test.txt", "2 2\n1 0\n0 2");

    CCSMatrix matrix;
    matrix.readAndPack("reconstruct_test.txt");

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    matrix.reconstructAndPrint();

    std::cout.rdbuf(old);
    std::string output = buffer.str();

    assert(output.find("1\t0") != std::string::npos);
    assert(output.find("0\t2") != std::string::npos);
}

void testEmptyReconstruction() {
    CCSMatrix matrix;

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    matrix.reconstructAndPrint();

    std::cout.rdbuf(old);
    std::string output = buffer.str();

    assert(output.find("Матрица не инициализирована") != std::string::npos);
}

int main() {
    testReconstruction();
    testEmptyReconstruction();
    return 0;
}