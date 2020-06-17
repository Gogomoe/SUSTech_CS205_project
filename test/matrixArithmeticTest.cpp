#include "../src/matrix.hpp"

bool determinantTest() {
    matrix::Matrix<int> m1(2, 2);
    matrix::Matrix<int> m2(3, 3);
    int vals1[4] = {1, 2, 2, 1};
    int vals2[9] = {2, -3, 1, 2, 0, -1, 1, 4, 5};
    m1.set(4, vals1);
    m2.set(9, vals2);

    assert(m1.determinant(m1, 2) == -3);
    assert(m2.determinant(m2, 3) == 49);

    return true;
}

bool inverseTest() {
    matrix::Matrix<int> m1(2, 2);
    matrix::Matrix<int> m2(2, 2);
    int vals[4] = {4, 3, 3, 2};
    m1.set(4, vals);

    m2 = m1.inverse();

    assert(m2.unsafe(0, 0) == -2);
    assert(m2.unsafe(0, 1) == 3);
    assert(m2.unsafe(1, 0) == 3);
    assert(m2.unsafe(1, 1) == -4);

    return true;
}