#include <iostream>
#include <random>
#include <opencv2/highgui/highgui.hpp>
#include <iomanip>
#include "matrix.h"

using namespace matrix;
using namespace std;

void testMatrix();

void testVector();

void testOpencvMat();

int main() {

    testMatrix();
    testVector();
    testOpencvMat();

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

    for (int i = 0; i < mat.getRows(); ++i) {
        for (int j = 0; j < mat.getCols(); ++j) {
            cout << mat[i][j] << " ";
        }
        cout << endl;
    }
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
