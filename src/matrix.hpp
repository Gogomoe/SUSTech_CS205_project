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
        Matrix(int rows, int cols) : rows_(rows), cols_(cols), mat_ptr_(new T[rows * cols]) {}

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

        template<class E>
        friend Matrix<E> &operator+(Matrix<E> const &, Matrix<E> const &); // addition

        template<class E>
        friend Matrix<E> &operator-(Matrix<E> const &, Matrix<E> const &); // minus

        Matrix<T> &operator-(Matrix<T> &); // unary minus

        template<class E>
        friend Matrix<E> &operator*(E const &, Matrix<E> const &); // scalar multiplication

        template<class E>
        friend Matrix<E> &operator*(Matrix<E> const &, E const &); // scalar multiplication

        template<class E>
        friend Matrix<E> &operator/(Matrix<E> const &, E const &); // scalar division

        Matrix<T>& transposition();

        Matrix<T>& conjugation();

        Matrix<T>& ewMul(Matrix<T> &other); // element-wise multiplication.

        Matrix<T>& mmMul(Matrix<T> &other); // matrix-matrix multiplication.
    };
}

#endif //CS205_PROJECT_MATRIX_HPP
