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

    std::stringstream buffer;           // �������������� �����
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    matrix.printArrays();
    std::cout.rdbuf(old);               // ��������������� ����������� �����

    std::string output = buffer.str();  // �������� � ��������� �����
    assert(output.find("������ ������� ��������� (AD): 1 3 5") != std::string::npos);

    return 0;
}