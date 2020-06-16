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