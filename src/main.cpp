#include <iostream>
#include <random>
#include "matrix.h"

int main() {

    using namespace matrix;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis{};

    Matrix<int> mat(2, 3);
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 3; ++j) {
            mat[i][j] = dis(gen) % 10;
        }
    }

    for (int i = 0; i < mat.getRows(); ++i) {
        for (int j = 0; j < mat.getCols(); ++j) {
            std::cout << mat[i][j] << " ";
        }
        std::cout << std::endl;
    }

    auto *p = new Matrix<int>(2, 3);
    Vector<int> v = (*p)[1];
    delete p;
    std::cout << v[2] << std::endl;

    return 0;
}
