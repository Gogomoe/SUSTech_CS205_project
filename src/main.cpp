#include <iostream>
#include <random>
#include <opencv2/highgui/highgui.hpp>
#include <iomanip>
#include <chrono>

#include "matrix.hpp"
#include "../test/test.hpp"
#include "../test/benchmark.hpp"

using namespace matrix;
using namespace std;

void testMatrix();

void testVector();

void testOpencvMat();

void testAdd();

void testEqual();

void testDenseCompute();

void testSlice();

void testConv();

void testPad();

template<typename T>
void printMatrix(const Matrix<T> &mat) {
    for (int i = 0; i < mat.getRows(); ++i) {
        for (int j = 0; j < mat.getCols(); ++j) {
            cout << mat.get(i, j) << " ";
        }
        cout << endl;
    }
}

int main() {
    benchmark();
    testMatrix();
    testVector();
    testOpencvMat();
    testAdd();
    testEqual();
    testSlice();
    // testDenseCompute();
    testConv();
    testPad();

    // test transport
    transpositionTest();
    matrixConjugationTest();

    // test arithmetic
    determinantTest();
    inverseTest();
    traceTest();

    return 0;
}

void testMatrix() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis{};

    Matrix<int> mat(2, 3);
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 3; ++j) {
            mat[i][j] = dis(gen) % 10;
        }
    }

    printMatrix(mat);
}

void testVector() {
    try {
        auto *p = new Matrix<int>(2, 3);
        Vector<int> v = (*p)[1];
        delete p;
        cout << v[2] << endl;
    } catch (exception &e) {
        cout << "catch " << e.what() << endl;
    }
}

void testOpencvMat() {
    using namespace cv;

    Mat_<int> img = imread("image.png", 0);
    Matrix<int> temp(img);
    Mat_<int> img2 = (Mat_<int>) temp;

    cout << "witdh: " << img2.cols << "  height: " << img2.rows << endl;

    ios_base::fmtflags default_flags(cout.flags());

    cout << hex << setfill('0') << setw(2);
    for (int i = 0; i < img2.rows; ++i) {
        for (int j = 0; j < img2.cols; ++j) {
            cout << hex << setfill('0') << setw(2) << img2(i, j);
        }
        cout << endl;
    }

    cout.flags(default_flags);
}

void testAdd() {
    Matrix<int> mat1(2, 3);
    Matrix<int> mat2(2, 3);
    mat1[0][0] = 0;
    mat1[0][1] = 1;
    mat1[0][2] = 2;

    mat2[1][0] = 3;
    mat2[1][1] = 4;
    mat2[1][2] = 5;

    Matrix<int> sum = mat1 + mat2;

    printMatrix(sum);

    Matrix<int> diff = mat1 - mat2;

    printMatrix(diff);
}


void testEqual() {
    Matrix<int> mat1(2, 3);
    Matrix<int> mat2(2, 3);
    mat1[0][0] = 0;
    mat1[0][1] = 1;
    mat1[0][2] = 2;

    mat2[0][0] = 0;
    mat2[0][1] = 1;
    mat2[0][2] = 2;

    cout << (mat1 == mat2) << endl;
}

void testDenseCompute() {
    const int rows = 2500;
    const int cols = 2500;

    Matrix<double> mat1(rows, cols);
    Matrix<double> mat2(rows, cols);

    double sum;

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            mat1.unsafe(i, j) = i;
            mat2.unsafe(i, j) = j;
        }
    }

    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    for (int i = 0; i < 1; ++i) {
        sum = (mat1.matmul(mat2)).sum();
    }
    chrono::steady_clock::time_point end = chrono::steady_clock::now();

    cout << "Result: " << sum << endl;
    cout << "Elapsed time: " << chrono::duration_cast<chrono::milliseconds> (end - begin).count() << "ms" << endl;
}

void testSlice() {
    Matrix<int> mat(3, 3);
    mat[0][0] = 0;
    mat[0][1] = 1;
    mat[0][2] = 2;
    mat[1][0] = 3;
    mat[1][1] = 4;
    mat[1][2] = 5;
    mat[2][0] = 6;
    mat[2][1] = 7;
    mat[2][2] = 8;

    printMatrix(mat.sliceRow(0, -1, 2));
    printMatrix(mat.sliceCol(0, -1, -1));

    printMatrix(mat.slice(0, 2, 2, 0, 2, 2));
}

void testConv() {
    const int rows = 5;
    const int cols = 5;

    Matrix<double> mat(rows, cols);
    Matrix<double> knl(3, 3);

    for (int i = 0; i < rows*cols; ++i) {
        mat.unsafe(i) = i;
    }

    for (int i = 0; i < 9; ++i) {
        knl.unsafe(i) = i;
    }

    printMatrix(mat);
    printMatrix(mat.convolve(knl));
}

void testPad() {
    const int rows = 5;
    const int cols = 5;

    Matrix<double> mat(rows, cols);

    for (int i = 0; i < rows*cols; ++i) {
        mat.unsafe(i) = i;
    }

    printMatrix(mat.pad(2, 2));
}