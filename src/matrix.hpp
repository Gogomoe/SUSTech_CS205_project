#ifndef CS205_PROJECT_MATRIX_HPP
#define CS205_PROJECT_MATRIX_HPP

#include <memory>
#include <opencv2/core/mat.hpp>

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
        // Initial matrix with specific rows and columns.
        Matrix(int rows, int cols) : rows_(rows), cols_(cols), mat_ptr_(new T[rows * cols]()) {}

        explicit Matrix(const cv::Mat_<T> &mat) : Matrix(mat.rows, mat.cols) {
            for (int row = 0; row < rows_; ++row) {
                for (int col = 0; col < cols_; ++col) {
                    mat_ptr_[row * cols_ + col] = mat(row, col);
                }
            }
        }

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

        const T &get(int row, int col) const {
            checkBound(row, 0, rows_);
            checkBound(col, 0, cols_);
            return mat_ptr_[row * cols_ + col];
        }

        inline T &unsafe(int row, int col) {
            return mat_ptr_[row * cols_ + col];
        }

        inline const T &unsafe(int row, int col) const {
            return mat_ptr_[row * cols_ + col];
        }

        Vector<T> operator[](int row) {
            checkBound(row, 0, rows_);
            return Vector<T>(row, cols_, mat_ptr_);
        }

        explicit operator cv::Mat_<T>() const {
            cv::Mat_<T> mat(rows_, cols_);
            for (int row = 0; row < rows_; ++row) {
                for (int col = 0; col < cols_; ++col) {
                    mat(row, col) = mat_ptr_[row * cols_ + col];
                }
            }
            return mat;
        }

        // add
        friend Matrix<T> operator+(const Matrix<T> &first, const Matrix<T> &second) {
            if (first.getCols() != second.getCols() || first.getRows() != second.getRows()) {
                throw std::length_error("The matrices don't have the same size!");
            }

            int rows = first.getRows();
            int cols = first.getCols();
            matrix::Matrix<T> result(rows, cols);

            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    result.unsafe(i, j) = first.unsafe(i, j) + second.unsafe(i, j);
                }
            }
            return result;
        }

        // minus
        friend Matrix<T> operator-(const Matrix<T> &first, const Matrix<T> &second) {
            if (first.getCols() != second.getCols() || first.getRows() != second.getRows()) {
                throw std::length_error("The matrices don't have the same size!");
            }

            int rows = first.getRows();
            int cols = first.getCols();
            matrix::Matrix<T> result(rows, cols);

            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    result.unsafe(i, j) = first.unsafe(i, j) - second.unsafe(i, j);
                }
            }
            return result;
        }

        // unary minus
        Matrix<T> operator-() const {
            matrix::Matrix<T> result(rows_, cols_);
            for (int i = 0; i < rows_; ++i) {
                for (int j = 0; j < cols_; ++j) {
                    result.unsafe(i, j) = -unsafe(i, j);
                }
            }
            return result;
        }

        // scalar multiplication
        friend Matrix<T> operator*(const T &, const Matrix<T> &);

        // scalar multiplication
        friend Matrix<T> operator*(const Matrix<T> &, const T &);

        // scalar division
        friend Matrix<T> operator/(const Matrix<T> &, const T &);

        Matrix<T> transposition() const;

        Matrix<T> conjugation() const;

        // element-wise multiplication.
        Matrix<T> ewMul(const Matrix<T> &other) const;

        // matrix-matrix multiplication.
        Matrix<T> mmMul(const Matrix<T> &other) const;
    };
}

#endif //CS205_PROJECT_MATRIX_HPP
