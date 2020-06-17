#ifndef CS205_BENCHMARK_HPP
#define CS205_BENCHMARK_HPP

#include <cstdio>
#include <cstring>
#include <chrono>
#include <vector>

#include "../src/matrix.hpp"

using namespace matrix;
using namespace std;

#define TEST_TIMEOUT 0.1E6
#define MAT_MAX_SIZE 2E9 // 4GB
typedef float base_t;

struct shape_t {
    size_t h, w;
    shape_t(size_t h, size_t w) : h(h), w(w) {}
};

struct result_t {
    const char *desc;
    shape_t s1, s2;
    long duration;
    base_t checksum;
    result_t(const char *desc, shape_t s1, shape_t s2, long duration, base_t checksum) : 
            desc(desc), s1(s1), s2(s2), duration(duration), checksum(checksum) {}
};

inline char* tail(char *str) {
    return &str[strlen(str)];
}

class test_item_t{
private:
    Matrix<base_t> (*func)(Matrix<base_t>, Matrix<base_t>);
public:
    vector<result_t> result;
    const char *desc;
    test_item_t(Matrix<base_t> (*func)(Matrix<base_t>, Matrix<base_t>),
            const char *desc) :
            func(func), desc(desc) {}

    Matrix<base_t> gen_mat(shape_t shape) {
        int rows = shape.h;
        int cols = shape.w;
        Matrix<base_t> mat(rows, cols);
        #pragma omp parallel for
        for (int i = 0; i < rows*cols; ++i) {
            mat.unsafe(i) = i;
        }
        return mat;
    }

    void auto_bench(shape_t init_1, shape_t incr_rate_1, shape_t init_2, shape_t incr_rate_2) {
        shape_t s1 = init_1;
        shape_t s2 = init_2;
        for(;;) {
            size_t mat_size = (s1.h*s1.w + s2.h*s2.w)*sizeof(base_t);
            if(mat_size>MAT_MAX_SIZE) break;

            Matrix<base_t> mat1 = gen_mat(s1);
            Matrix<base_t> mat2 = gen_mat(s2);

            printf("[%s]<input: (%ld, %ld), (%ld, %ld), size: %.2gGB> begin\n", desc, s1.h, s1.w, s2.h, s2.w, mat_size/1E9);
            chrono::steady_clock::time_point begin = chrono::steady_clock::now();
            Matrix<base_t> ret = func(mat1, mat2);
            chrono::steady_clock::time_point end = chrono::steady_clock::now();
            long duration = chrono::duration_cast<chrono::microseconds> (end - begin).count();
            base_t checksum = ret.sum();

            cout << "finish in " << duration << "us" << endl;
            cout << "<checksum: " << ret.sum() <<">" << endl << endl;

            result.push_back(result_t(desc, s1, s2, duration, checksum));

            if(duration>TEST_TIMEOUT) break;

            s1.h *= incr_rate_1.h;
            s1.w *= incr_rate_1.w;
            s2.h *= incr_rate_2.h;
            s2.w *= incr_rate_2.w;
        }
    }
};

Matrix<base_t> _opt_add(Matrix<base_t> a, Matrix<base_t> b) {
    return a+b;
}

Matrix<base_t> _opt_sum(Matrix<base_t> a, Matrix<base_t> b) {
    Matrix<base_t> ret(1, 1, false);
    ret.unsafe(0) = a.sum();
    return ret;
}

Matrix<base_t> _opt_matmul(Matrix<base_t> a, Matrix<base_t> b) {
    return a.matmul(b);
}

Matrix<base_t> _opt_conv(Matrix<base_t> a, Matrix<base_t> b) {
    return a.convolve(b);
}

void benchmark_group(vector<test_item_t> &items, shape_t init_1, shape_t incr_rate_1, shape_t init_2, shape_t incr_rate_2) {
    for(size_t i=0; i<items.size(); ++i) {
        items[i].auto_bench(init_1, incr_rate_1, init_2, incr_rate_2);
    }
}

void dump_report(vector<test_item_t> items, const char *filename) {
    FILE *fp = fopen(filename, "w");
    
    for(test_item_t item : items) {
        fprintf(fp, "%s,", item.desc);
        for(result_t r : item.result) {
            fprintf(fp, "%ld,", r.duration);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
}

void benchmark() {
    shape_t init(10, 10);
    shape_t incr(2, 2);
    shape_t knl3x3(3, 3);
    shape_t nochange(1, 1);

    vector<test_item_t> group1 = {
        test_item_t(_opt_add, "add"),
        test_item_t(_opt_sum, "sum"),
        test_item_t(_opt_matmul, "matmul"),
    };

    benchmark_group(group1, init, incr, init, incr);
    dump_report(group1, "g1.csv");

    vector<test_item_t> group2 = {
        test_item_t(_opt_conv, "conv")
    };

    benchmark_group(group2, init, incr, knl3x3, nochange);
    dump_report(group2, "g2.csv");

    exit(0);
}
#endif