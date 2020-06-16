#ifndef CS205_COMPLEX_HPP
#define CS205_COMPLEX_HPP

#include <iostream>

class Complex {
public:
    Complex();
    Complex(double re, double im);

    Complex operator~();
    Complex operator+(const Complex&);
    Complex operator-(const Complex&);
    Complex operator*(const Complex&);
    bool operator==(const Complex&);
    bool operator!=(const Complex&);

    friend std::ostream& operator<<(std::ostream &, const Complex&);
    friend void operator>>(std::istream &, Complex&);
    friend Complex operator*(int, Complex&);

private:
    double imag;
    double real;
};

#endif //CS205_COMPLEX_HPP
