//
// Created by Artem on 13.05.2026.
//

#ifndef DD_POW_EXACT_MATH_H
#define DD_POW_EXACT_MATH_H

#include <cmath>
//по большей части старался опираться на статью A Floating-Point Technique for Extending the Available Precision. T. J. DEKKER
namespace e_math {
    //реализация exact_addition через универсальный алгоитм Мёллера-Кнута
    inline double add2(double x, double y, double &err) {
        double z = x + y;
        double w = z - x;
        double v = z - w;
        err = y - w - (v - x);
        return z;
    }

    //да, быстрая суммма была предложена не в работе Деккера (у Шевчюка), но стиль названий буду использвовать оттуда. Реализация быстрой суммы, работатет только для
    // |x| >= |y|
    inline double quick_add2(double x, double y, double &err) {
        double s = x + y;
        err = y - (s - x);
        return s;
    }

    inline double sub2(double x, double y, double &err) {
        double z = x - y;
        double w = x - z;
        double v = z + w;
        err = y - w - (v - x);
        return z;
    }


    inline double mul2(double x, double y, double &err) {
        double z = x * y;
        err = std::fma(x, y, -z);
        return z;
    }


    inline double square(double x, double &err) {
        double z = x * x;
        err = std::fma(x, x, -z);
        return z;
    }
}


#endif //DD_POW_EXACT_MATH_H
