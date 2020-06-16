#ifndef CS205_PROJECT_MATRIX_HPP
#define CS205_PROJECT_MATRIX_HPP

#include <memory>
#include <complex>
#include <opencv2/core/mat.hpp>

#include "Complex.hpp"

namespace matrix {

    void checkBound(int it, int lower, int upper);

    std::vector<int> makeSlice(int start, int end, int step, int upperBound);

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
        int rows_;
        int cols_;
        std::shared_ptr<T[]> mat_ptr_;
    public:
        // Initial matrix with specific rows and columns.
        Matrix(int rows, int cols) : rows_(rows), cols_(cols), mat_ptr_(new T[rows * cols]()) {}

        Matrix(const Matrix<T> &mat) : Matrix(mat.rows_, mat.cols_) {
            for (int i = 0; i < rows_; ++i) {
                for (int j = 0; j < cols_; ++j) {
                    unsafe(i, j) = mat.unsafe(i, j);
                }
            }
        }

        Matrix(Matrix<T> &&mat) noexcept: rows_(mat.rows_), cols_(mat.cols_), mat_ptr_(mat.mat_ptr_) {}

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
            return *this;
        }

        Matrix<T> &set(int length, T *vals) {
            for (int i = 0; i < length; ++i) {
                mat_ptr_[i] = *vals;
                ++vals;
            }
            return *this;
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

        Matrix<T> &operator=(const Matrix<T> &other) {
            if (this == other) {
                return *this;
            }
            rows_ = other.rows_;
            cols_ = other.cols_;
            mat_ptr_.reset(new T[rows_ * cols_]());
            for (int i = 0; i < rows_; ++i) {
                for (int j = 0; j < cols_; ++j) {
                    unsafe(i, j) = other.unsafe(i, j);
                }
            }
            return *this;
        }

        Matrix<T> &operator=(Matrix<T> &&other) noexcept {
            rows_ = other.rows_;
            cols_ = other.cols_;
            mat_ptr_ = other.mat_ptr_;
            return *this;
        }

        bool operator==(const Matrix<T> &other) const {
            if (rows_ != other.rows_ || cols_ != other.cols_) {
                return false;
            }

            bool ret = true;
            #pragma omp parallel for collapse(2)
            for (int i = 0; i < rows_; ++i) {
                for (int j = 0; j < cols_; ++j) {
                    if (!ret) continue;
                    if (unsafe(i, j) != other.unsafe(i, j)) {
                        ret = false;
                        // return false;
                    }
                }
            }
            return ret;
        }

        bool operator!=(const Matrix &rhs) const {
            return rhs != *this;
        }

        explicit operator cv::Mat_<T>() const {
            cv::Mat_<T> mat(rows_, cols_);
            #pragma omp parallel for collapse(2)
            for (int row = 0; row < rows_; ++row) {
                for (int col = 0; col < cols_; ++col) {
                    mat(row, col) = mat_ptr_[row * cols_ + col];
                }
            }
            return mat;
        }

        static void assertMatricesWithSameShape(const Matrix<T> &first, const Matrix<T> &second) {
            if (first.getCols() != second.getCols() || first.getRows() != second.getRows()) {
                throw std::length_error("The matrices don't have the same shape!");
            }
        }

        static void assertMatricesWithSameSize(const Matrix<T> &first, const Matrix<T> &second) {
            if (first.getCols() * first.getRows() != second.getCols() * second.getRows()) {
                throw std::length_error("The matrices don't have the same size!");
            }
        }

        // add
        friend Matrix<T> operator+(const Matrix<T> &first, const Matrix<T> &second) {
            Matrix<T>::assertMatricesWithSameShape(first, second);

            int rows = first.getRows();
            int cols = first.getCols();
            matrix::Matrix<T> result(rows, cols);

            #pragma omp parallel for collapse(2)
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    result.unsafe(i, j) = first.unsafe(i, j) + second.unsafe(i, j);
                }
            }
            return result;
        }

        // minus
        friend Matrix<T> operator-(const Matrix<T> &first, const Matrix<T> &second) {
            Matrix<T>::assertMatricesWithSameShape(first, second);

            int rows = first.getRows();
            int cols = first.getCols();
            matrix::Matrix<T> result(rows, cols);

            #pragma omp parallel for collapse(2)
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
            #pragma omp parallel for collapse(2)
            for (int i = 0; i < rows_; ++i) {
                for (int j = 0; j < cols_; ++j) {
                    result.unsafe(i, j) = -unsafe(i, j);
                }
            }
            return result;
        }

        // scalar multiplication
        friend Matrix<T> operator*(const T &val, const Matrix<T> &mat) {
            int rows = mat.getRows();
            int cols = mat.getCols();
            matrix::Matrix<T> result(rows, cols);

            #pragma omp parallel for collapse(2)
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    result.unsafe(i, j) = mat.unsafe(i, j) * val;
                }
            }
            return result;
        }

        // scalar multiplication
        friend Matrix<T> operator*(const Matrix<T> &mat, const T &val) {
            int rows = mat.getRows();
            int cols = mat.getCols();
            matrix::Matrix<T> result(rows, cols);

            #pragma omp parallel for collapse(2)
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    result.unsafe(i, j) = mat.unsafe(i, j) * val;
                }
            }
            return result;
        }

        // scalar division
        friend Matrix<T> operator/(const Matrix<T> &mat, const T &val) {
            int rows = mat.getRows();
            int cols = mat.getCols();
            matrix::Matrix<T> result(rows, cols);

            #pragma omp parallel for collapse(2)
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    result.unsafe(i, j) = mat.unsafe(i, j) / val;
                }
            }
            return result;
        }

        Matrix<T> transposition() const {
            matrix::Matrix<T> result(cols_, rows_);

            #pragma omp parallel for collapse(2)
            for (int i = 0; i < rows_; ++i) {
                for (int j = 0; j < cols_; ++j) {
                    result.unsafe(j, i) = unsafe(i, j);
                }
            }
            return result;
        }

        Matrix<Complex> conjugation() const {
            matrix::Matrix<Complex> result = transposition();

            #pragma omp parallel for collapse(2)
            for (int i = 0; i < rows_; ++i) {
                for (int j = 0; j < cols_; ++j) {
                    const Complex &value = result.unsafe(j, i);
                    result.unsafe(j, i) = Complex(value.getReal(), value.getImag());
                }
            }
            return result;
        }

        // element-wise multiplication.
        Matrix<T> multiply(const Matrix<T> &other) const {
            Matrix<T>::assertMatricesWithSameShape(*this, other);

            int rows = getRows();
            int cols = getCols();
            matrix::Matrix<T> result(rows, cols);

            #pragma omp parallel for collapse(2)
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    result.unsafe(i, j) = unsafe(i, j) * other.unsafe(i, j);
                }
            }
            return result;
        }

        // matrix-matrix multiplication.
        Matrix<T> matmul(const Matrix<T> &other) const {
            if (getCols() != other.getRows()) {
                throw std::length_error("Matrices with incompatible shapes cannot perform matmul!");
            }

            int rows = getRows();
            int cols = other.getCols();

            matrix::Matrix<T> result(rows, cols);

            #pragma omp parallel for collapse(3)
            for (int i = 0; i < rows; ++i) { // reorder for loops to avoid stride memory access
                for (int j = 0; j < getCols(); ++j) {
                    for (int k = 0; k < cols; ++k) {
                        result.unsafe(i, k) += unsafe(i, j) * other.unsafe(j, k);
                    }
                }
            }
            return result;
        }

        T sum() const {
            int rows = getRows();
            int cols = getCols();

            T sumVal = T();

            #pragma omp parallel for reduction(+: sumVal) collapse(2)
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    sumVal += unsafe(i, j);
                }
            }

            return sumVal;
        }

        Matrix<T> sliceRow(int start = 0, int end = -1, int step = 1) {
            return sliceRow(makeSlice(start, end, step, rows_));
        }

        Matrix<T> sliceRow(const std::vector<int> &rows) {
            Matrix<T> result(rows.size(), cols_);
            for (int i = 0; i < rows.size(); ++i) {
                int row = rows[i];
                checkBound(row, 0, rows_);
                for (int j = 0; j < cols_; ++j) {
                    result.unsafe(i, j) = unsafe(row, j);
                }
            }
            return result;
        }

        Matrix<T> sliceCol(int start = 0, int end = -1, int step = 1) {
            return sliceCol(makeSlice(start, end, step, cols_));
        }

        Matrix<T> sliceCol(const std::vector<int> &cols) {
            Matrix<T> result(rows_, cols.size());
            for (int i = 0; i < cols.size(); ++i) {
                int col = cols[i];
                checkBound(col, 0, cols_);
                for (int j = 0; j < rows_; ++j) {
                    result.unsafe(j, i) = unsafe(j, col);
                }
            }
            return result;
        }

        Matrix<T> slice(
                int rowStart = 0, int rowEnd = -1, int rowStep = 1,
                int colStart = 0, int colEnd = -1, int colStep = 1
        ) {
            return slice(
                    makeSlice(rowStart, rowEnd, rowStep, rows_),
                    makeSlice(colStart, colEnd, colStep, cols_)
            );
        }

        Matrix<T> slice(const std::vector<int> &rows, const std::vector<int> &cols) {
            Matrix<T> result(rows.size(), cols.size());
            for (int row : rows) {
                checkBound(row, 0, rows_);
            }
            for (int col : cols) {
                checkBound(col, 0, cols_);
            }
            for (int i = 0; i < rows.size(); ++i) {
                int row = rows[i];
                for (int j = 0; j < cols.size(); ++j) {
                    int col = cols[j];
                    result.unsafe(i, j) = unsafe(row, col);
                }
            }
            return result;
        }
    };
}

#endif //CS205_PROJECT_MATRIX_HPP
