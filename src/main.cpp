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

    for (int i = 0; i < mat.getRow(); ++i) {
        for (int j = 0; j < mat.getCol(); ++j) {
            std::cout << mat[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
