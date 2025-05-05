#include "../SparseMatrix.h"
#include <cassert>
#include <fstream>

void createTestFile(const char* filename, const char* content) {
    std::ofstream file(filename);
    file << content;
    file.close();
}

void testReadValidMatrix() {
    createTestFile("valid.txt", "2 2 1 0 0 2");

    SparseMatrix matrix;
    assert(matrix.readAndPack("valid.txt") == true);
    std::remove("valid.txt");
}

void testReadInvalidFile() {
    SparseMatrix matrix;
    assert(matrix.readAndPack("nonexistent.txt") == false);
}

void testReadInvalidData() {
    createTestFile("invalid.txt", "2 2 1 a 0 2");

    SparseMatrix matrix;
    assert(matrix.readAndPack("invalid.txt") == false);
    std::remove("invalid.txt");
}

void testReadExtraData() {
    createTestFile("extra.txt", "2 2 1 0 0 2 5");

    SparseMatrix matrix;
    assert(matrix.readAndPack("extra.txt") == false);
    std::remove("extra.txt");
}

int main() {
    testReadValidMatrix();
    testReadInvalidFile();
    testReadInvalidData();
    testReadExtraData();
    return 0;
}