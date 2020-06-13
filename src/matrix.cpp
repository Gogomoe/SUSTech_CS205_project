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

template<class E>
matrix::Matrix<E> &operator+(matrix::Matrix<E> const &first, matrix::Matrix<E> const &second) {
    if (first.getCols() != second.getCols() || first.getRows() != second.getRows())
        throw std::length_error("The matrices don't have the same size!");
    int row = first.getRows();
    int col = first.getCols();
    matrix::Matrix<E> result(row, col);
    for (int i = 0; i < row; ++i)
        for (int j = 0; j < col; ++j)
            result.set(i, j, first.get(i, j) + second.get(i, j));
    return &result;
}

template<class E>
matrix::Matrix<E> &operator-(matrix::Matrix<E> const &first, matrix::Matrix<E> const &second) {
    if (first.getCols() != second.getCols() || first.getRows() != second.getRows())
        throw std::length_error("The matrices don't have the same size!");
    int row = first.getRows();
    int col = first.getCols();
    matrix::Matrix<E> result(row, col);
    for (int i = 0; i < row; ++i)
        for (int j = 0; j < col; ++j)
            result.set(i, j, first.get(i, j) - second.get(i, j));
    return &result;
}

template<typename T>
matrix::Matrix<T> &operator-(matrix::Matrix<T> &first) {
    int row = first.getRows();
    int col = first.getCols();
    matrix::Matrix<T> result(row, col);
    for (int i = 0; i < row; ++i)
        for (int j = 0; j < col; ++j)
            result.set(i, j, -first.get(i, j));
    return &result;
}

template<class E>
matrix::Matrix<E> &operator*(E const &first, matrix::Matrix <E> const &second) {
    return nullptr;
}

template<class E>
matrix::Matrix<E> &operator*(matrix::Matrix <E> const &first, E const &second) {
    return nullptr;
}

template<class E>
matrix::Matrix<E> &operator/(matrix::Matrix <E> const &first, E const &second) {
    return nullptr;
}

template<typename T>
matrix::Matrix<T> &matrix::Matrix<T>::transposition() {
    return nullptr;
}

template<typename T>
matrix::Matrix<T> &matrix::Matrix<T>::conjugation() {
    return nullptr;
}

template<typename T>
matrix::Matrix<T> &matrix::Matrix<T>::ewMul(matrix::Matrix<T> &other) {
    return nullptr;
}

template<typename T>
matrix::Matrix<T> &matrix::Matrix<T>::mmMul(matrix::Matrix<T> &other) {
    return nullptr;
}