#include <cassert>

#include "../src/matrix.hpp"

bool matrixTransportationTest() {
    matrix::Matrix<int> m1(2, 3);
    matrix::Matrix<int> m2(3, 2);
    int vals[6] = {1, 2, 3, 4, 5, 6};

    m1.set(6, vals);
    m2 = m1.transposition();

    assert(m2.get(0, 0) == 1);
    assert(m2.get(0, 1) == 4);
    assert(m2.get(1, 0) == 2);
    assert(m2.get(1, 1) == 5);
    assert(m2.get(2, 0) == 3);
    assert(m2.get(2, 1) == 6);

    return true;
}

bool matrixConjugationTest() {
    matrix::Matrix<Complex> m1(2, 3);
    matrix::Matrix<Complex> m2(3, 2);
    Complex vals[6];
    vals[0] = Complex(1, 0), vals[1] = Complex(2, 1), vals[2] = Complex(3, -1);
    vals[3] = Complex(4, 0), vals[4] = Complex(5, 1), vals[5] = Complex(6, -1);

    m1.set(6, vals);
    m2 = m1.conjugation();

    assert(m2.get(0, 0) == Complex(1, 0));
    assert(m2.get(0, 1) == Complex(4, 0));
    assert(m2.get(1, 0) == Complex(2, -1));
    assert(m2.get(1, 1) == Complex(5, -1));
    assert(m2.get(2, 0) == Complex(3, 1));
    assert(m2.get(2, 1) == Complex(6, 1));

    return true;
}