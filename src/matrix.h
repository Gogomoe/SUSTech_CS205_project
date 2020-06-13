#ifndef CS205_PROJECT_MATRIX_H
#define CS205_PROJECT_MATRIX_H

#include <memory>

namespace matrix {

    void checkBound(int it, int lower, int upper);

    template<typename T>
    class Vector {
    private:
        int row_;
        int cols_;
        std::weak_ptr<T[]> mat_ptr_;
    public:
        Vector(int row, int cols, std::weak_ptr<T[]> mat_ptr) : row_(row), cols_(cols), mat_ptr_(mat_ptr) {}

        T &operator[](int col) {
            checkBound(col, 0, cols_);
            if (mat_ptr_.expired()) {
                throw std::bad_weak_ptr();
            }
            return mat_ptr_.lock()[row_ * cols_ + col];
        }
    };

    template<typename T>
    class Matrix {
    private:
        int cols_;
        int rows_;
        std::shared_ptr<T[]> mat_ptr_;
    public:
        Matrix(int rows, int cols) : rows_(rows), cols_(cols), mat_ptr_(new T[rows * cols]) {}

        ~Matrix() {
            mat_ptr_.reset();
        }

        [[nodiscard]] int getCols() const {
            return cols_;
        }

        [[nodiscard]] int getRows() const {
            return rows_;
        }

        Matrix<T> &set(int row, int col, T val) {
            checkBound(row, 0, rows_);
            checkBound(col, 0, cols_);
            mat_ptr_[row * cols_ + col] = val;
            return this;
        }

        T &get(int row, int col) {
            checkBound(row, 0, rows_);
            checkBound(col, 0, cols_);
            return mat_ptr_[row * cols_ + col];
        }

        Vector<T> operator[](int row) {
            checkBound(row, 0, rows_);
            return Vector<T>(row, cols_, mat_ptr_);
        }
    };

}

#endif //CS205_PROJECT_MATRIX_H
