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

    matrix.unpackAndPrint(); // ������ ������� � ������ ������ �� ����������

    std::cout.rdbuf(old);

    std::string output = buffer.str();
    assert(output.empty());  // ������ �� ����������
}

void testSingleNonZeroElement() {
    SparseMatrix matrix;
    int A[] = { 5 };         // ���� ��������� �������
    int LI[] = { 0 };        // ������ ������
    int LJ[] = { 1 };        // ������ �������

    matrix.fillManual(2, 2, 1, A, LI, LJ);         // ������� 2x2, 1 ��������� �������

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    matrix.unpackAndPrint();                       // �������, ��� ���������� ����� ������ 5 � ������� (0, 1)

    std::cout.rdbuf(old);

    std::string output = buffer.str();
    assert(output.find("5") != std::string::npos);
    assert(output.find("1") == std::string::npos); // ������ � ������ �� ������ ����������
}

int main() {
    testUnpackValid();
    testUnpackEmpty();
    testSingleNonZeroElement();
    return 0;
}
