//
// Created by Artem on 13.05.2026.
//

#ifndef DD_POW_DD_H
#define DD_POW_DD_H
#include "exact_math.h"

struct dd {
    double h, t; // head, tail

    dd(double h, double t) : h(h), t(t) {}
    dd() : h(0.0), t(0.0) {}
    dd(double h) : h(h), t(0.0) {}

    dd operator+(const dd& other) const;
    dd operator-(const dd &other) const;
    dd operator*(const dd& other) const;
    dd operator/(const dd& other) const;

    dd operator+(double a) const;
    dd operator-(double a) const;
    dd operator*(double a) const;
    dd operator/(double a) const;
};

dd square_dd(const dd& a);
dd ldexp_dd(const dd& a, int exp);


#endif //DD_POW_DD_H
