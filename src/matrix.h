#ifndef CS205_PROJECT_MATRIX_H
#define CS205_PROJECT_MATRIX_H

namespace matrix {

    void check_bound(int it, int lower, int upper);

    template<typename T>
    class Row {
    private:
        int row_;
        int col_;
        T *mat_ptr_;
    public:
        Row(int row, int col, T *rowPtr) : row_(row), col_(col), mat_ptr_(rowPtr) {}

        T &operator[](int col) {
            check_bound(col, 0, col_);
            return mat_ptr_[row_ * col_ + col];
        }
    };

    template<typename T>
    class Matrix {
    private:
        int col_;
        int row_;
        T *mat_ptr_;
    public:
        Matrix(int row, int col) : row_(row), col_(col), mat_ptr_(new T[row * col]) {}

        ~Matrix() {
            delete[] mat_ptr_;
        }

        int getCol() const {
            return col_;
        }

        int getRow() const {
            return row_;
        }

        Matrix<T> &set(int row, int col, T val) {
            check_bound(row, 0, row_);
            check_bound(col, 0, col_);
            mat_ptr_[row * col_ + col] = val;
            return this;
        }

        T &get(int row, int col) {
            check_bound(row, 0, row_);
            check_bound(col, 0, col_);
            return mat_ptr_[row * col_ + col];
        }

        Row<T> operator[](int row) {
            check_bound(row, 0, row_);
            return Row<T>(row, col_, mat_ptr_);
        }
    };

}

#endif //CS205_PROJECT_MATRIX_H
