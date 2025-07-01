#include "SSSMatrix.h"
#include <locale.h>

int main() {
    setlocale(LC_ALL, "Russian");
    SSSMatrix A;
    A.inputFromFileAndPackageMatrix("SSSMatrix.h");
    A.printArrays();
    std::cout << A.sumElements() << std::endl;
    return 0;
}