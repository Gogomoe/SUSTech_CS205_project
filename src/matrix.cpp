#include "matrix.hpp"
#include <stdexcept>

void matrix::checkBound(int it, int lower, int upper) {
    if (it < lower || it >= upper) {
        throw std::range_error(
                std::to_string(it) + " not in range ["
                + std::to_string(lower) + ","
                + std::to_string(upper) + ")"
        );
    }
}
