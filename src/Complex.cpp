#include <iostream>
#include "Complex.hpp"

using namespace std;

Complex::Complex() : real(0), imag(0) {}

Complex::Complex(double re, double im) : real(re), imag(im) {}

Complex Complex::operator~() {
    return {-real, -imag};
}

Complex Complex::operator+(const Complex &other) {
    return {real + other.real, imag + other.imag};
}

Complex Complex::operator-(const Complex &other) {
    return {real - other.real, imag - other.imag};
}

Complex Complex::operator*(const Complex &other) {
    double real_ = real * other.real - imag * other.imag;
    double imag_ = real * other.imag + imag * other.real;
    return {real_, imag_};
}

bool Complex::operator==(const Complex &other) {
    return real == other.real && imag == other.imag;
}

bool Complex::operator!=(const Complex &other) {
    return real != other.real || imag != other.imag;
}

ostream &operator<<(ostream &os, const Complex &other) {
    if (other.imag < 0) os << other.real << other.imag << "i";
    else if (other.imag == 0) os << other.real;
    else os << other.real << "+" << other.imag << "i";
    return os;
}

void operator>>(istream &is, Complex &other) {
    cout << "real: ";
    is >> other.real;
    cout << "imaginary: ";
    is >> other.imag;
}

Complex operator*(int scala, Complex &other) {
    return {scala * other.real, scala * other.imag};
}