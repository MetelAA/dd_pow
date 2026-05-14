//
// Created by Artem on 13.05.2026.
//

#include "dd.h"

//дословные реализации операций DDxDD взяты из Tight and Rigorous Error Bounds for Basic Building Blocks of Double-Word Arithmetic

dd dd::operator+(const dd &other) const { //ALGORITHM 6 - AccurateDWPlusDW
    double sh, sl;
    sh = e_math::add2(this->h, other.h, sl);
    double th, tl;
    th = e_math::add2(this->t, other.t, tl);
    double c = sl + th;
    double vh, vl;
    vh = e_math::quick_add2(sh, c, vl);
    double w = tl + vl;
    double head, tail;
    head = e_math::quick_add2(vh, w, tail);
    return dd{head, tail};
}

dd dd::operator-(const dd &other) const {
    double sh, sl;
    sh = e_math::sub2(this->h, other.h, sl);
    double th, tl;
    th = e_math::sub2(this->t, other.t, tl);
    double c = sl + th;
    double vh, vl;
    vh = e_math::quick_add2(sh, c, vl);
    double w = tl + vl;
    double head, tail;
    head = e_math::quick_add2(vh, w, tail);
    return dd{head, tail};
}

dd dd::operator*(const dd &other) const { //ALGORITHM 11 - DWTimesDW2
    double ch, cl1;
    ch = e_math::mul2(this->h, other.h, cl1);
    double cl3 = cl1 + ((this->h * other.t) + this->t * other.h);
    double head, tail;
    head = e_math::quick_add2(ch, cl3, tail);
    return dd{head, tail};
}

dd dd::operator/(const dd &other) const { // ALGORITHM 17 - DWDivDW2
    double th = this->h / other.h;
    dd r = dd{other.h, other.t} * th;
    double Pih = this->h - r.h;
    double bl = this->t - r.t;
    double b = Pih + bl;
    double tl = b / other.h;
    double head, tail;
    head = e_math::quick_add2(th, tl, tail);
    return dd{head, tail};
}

dd dd::operator+(double a) const { //ALGORITHM 4 - DWPlusFP
    double sh, sl;
    sh = e_math::add2(this->h, a, sl);
    double v = this->t + sl;
    double head, tail;
    head = e_math::quick_add2(sh, v, tail);
    return dd{head, tail};
}

dd dd::operator-(double a) const {
    double sh, sl;
    sh = e_math::sub2(this->h, a, sl);
    double v = this->t + sl;
    double head, tail;
    head = e_math::quick_add2(sh, v, tail);
    return dd{head, tail};
}

dd dd::operator*(double a) const { // ALGORITHM 8 - DWTimesFP2
    double ch, cl1;
    ch = e_math::mul2(this->h, a, cl1);
    double cl3 = cl1 + (this->t * a);
    double head, tail;
    head = e_math::quick_add2(ch, cl3, tail);
    return dd{head, tail};
}

dd dd::operator/(double a) const { // ALGORITHM 14 - DWDivFP2
    double th = this->h / a;
    double Pih, Pil;
    Pih = e_math::mul2(th, a, Pil);
    double bh = this->h - Pih;
    double bt = this->t - Pil;
    double b = bh + bt;
    double tl = b / a;
    double head, tail;
    head = e_math::quick_add2(th, tl, tail);
    return dd{head, tail};
}

dd square_dd(const dd &a) {
    return a * a;
}

dd ldexp_dd(const dd& a, int exp) {
    return dd(std::ldexp(a.h, exp), std::ldexp(a.t, exp));
}
