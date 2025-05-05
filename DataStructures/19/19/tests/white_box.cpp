#include <iostream>
#include <cassert>
#include <stdexcept>
#include <cmath>
#include <fstream>
#include "../19.h"

using namespace std;


// ���� 5: fillManual � reconstruct
void testFillManual() {
    double a[] = { 1.0, 2.0, 3.0 };
    int li[] = { 0, 0, 2 };
    int lj[] = { 0, 1, 2, 3 };

    CCSMatrix m;
    m.fillManual(3, 3, 3, a, li, lj);
    m.printCCS();
    m.reconstructAndPrint();
}

// ���� 6: sortColumnsBySum �� �������������������� �������
void testSortUninitialized() {
    CCSMatrix m;
    bool caught = false;
    try {
        m.sortColumnsBySum();
    }
    catch (const std::runtime_error& e) {
        caught = true;
    }
    assert(caught && "Expected exception on sort of uninitialized matrix");
}

int main() {
    cout << "=== White-box Testing CCSMatrix ===" << endl;
    cout << "5" << endl;
    testFillManual();
    cout << "6" << endl;
    testSortUninitialized();

    cout << "All tests passed successfully!" << endl;
    return 0;
}
