//
// Created by Artem on 13.05.2026.
//

#ifndef DD_POW_UTILS_H
#define DD_POW_UTILS_H

#include <string>
#include <sstream>
#include <iomanip>
#include "dd.h"

std::string to_string_dd(const dd& x) {
    if (std::isinf(x.h) || std::isnan(x.h)) {
        return std::to_string(x.h);
    }

    const int precision = 32;

    int exp_hi = 0;
    if (x.h != 0.0) {
        exp_hi = static_cast<int>(std::floor(std::log10(std::abs(x.h))));
    }

    double lo_scaled = x.t * std::pow(10.0, -exp_hi);

    std::ostringstream oss;
    oss << std::scientific << std::setprecision(precision) << x.h;
    oss << " + " << std::fixed << std::setprecision(precision)
        << lo_scaled << "e" << (exp_hi >= 0 ? "+" : "") << exp_hi;

    return oss.str();
}


dd ln_dd(const dd &x) { //ln(x)
    const dd SQRT1_2(0.7071067811865475, 0.0);
    const dd SQRT2(1.4142135623730951, 1.9051658006994172e-17);
    const double EPS = 4.93038065763132e-32;
    const dd LN2(0.6931471805599453, 2.3190468138462996e-17);

    //редуцируем аргумент x = 2^m*t, m - целое, t принадлежит [0.5, 1)
    //тогда ln(2^m*t) = m*ln(2) + ln(t), т.к. t недалеко от 1, то ряд которым приближаем логарифм быстро сходиться
    int m;
    double t0 = std::frexp(x.h, &m);
    dd t = x * std::ldexp(1.0, -m); //==x/2^m

    // позаимствовано из библиотеки Quad-Double Library, нужно для ещё большего прижатия t к интервалу [1/sqrt(2), sqrt(2)] - типо более оптимально чем [0.5, 1), т.к. U
    // будет иметь меньшее максимальное значение, а от U зависит скорость сходимости ряда
    if (t.h < SQRT1_2.h) {
        t = t * 2.0;
        m--;
    } else if (t.h > SQRT2.h) {
        t = t * 0.5;
        m++;
    }
    const dd one(1.0);
    dd U = (t - one) / (t + one);

    //далее считаем ряд ln(t)=2*(U+U^3/3+U^5/5+...), вычисляем так пока:
    //1. Пока n-ый член не станет меньше машинной точности (эпсилон)
    //2. Пока n не привысит константу (пока будет 25)
    dd lnt = U;
    dd step = square_dd(U);
    dd term = U;
    int n = 1; //одна иттерация уже типо была)

    while (n <= 25) {
        term = term * step;
        dd delta = term / (2 * n + 1);
        if (std::fabs(delta.h) < EPS) break;
        lnt = lnt + delta;
        n++;
    }
    lnt = lnt + lnt;

    //считаем итоговую формулу
    dd rez = LN2 * double(m) + lnt;
    return rez;
}

dd exp_dd(const dd &x) { //e^x
    //редуцируем/разлагаем x примерно также как в логарифме, x = m*ln(2)+r, m - двойка в какой-то степени
    //итоговая r-ка будет находиться в пределах [-ln(2)/2, ln(2)/2]
    //таким образом из e^x получаем e^(m*ln(2)+r) воспользовашись свойствами логарифма получаем e^(ln(2)*m)*e^r = 2^m*e^r, т.е. теперь необходимо посчитать e^r, что можно сделать, разложив в ряд тейлора
    //но мы уменьшим r ещё сильнее, теперь r = k*t, где k - двойка в какой-то не очень большой степени, встречаются 256 и 512, позволяет добиться ещё лучшей сходимости

    //переходя к практике, добавим ограничения на x, x принадлежит промежутку [-709, 709] на правой/левой границе он уходит или в inf или в 0, также для x=0.0 очев ответ 0 (крайний случай)
    const double EPS = 4.93038065763132e-32;
    const dd LN2(0.6931471805599453, 2.3190468138462996e-17);

    if (x.h > 709.0) return {INFINITY};
    if (x.h < -709.0) return {0.0};
    if (x.h == 0.0 && x.t == 0.0) return {1.0};

    int m = std::round((x /
                        LN2).h); // думаб что хвост на округление к целому повлиять не должен, учитывая что накопление ошибки в данный момент ещё не может возникнуть
    dd r = x - LN2 * m;

    int k = 512; //k=2^p, p = 9
    int p = 9;
    dd t = r / k;

    // далее считаем ряд Тейлора для e^t-1=t+ t^2/2! + t^3/3! + t^4/4! + ..., вычисляем так пока:
    //1. Пока n-ый член не станет меньше машинной точности (эпсилон)
    //2. Пока n не привысит константу (пока будет ?)

    dd term = t;
    dd s = t;
    int n = 1; //одна иттерация уже типо была)
    while (n <= 35 && std::fabs(term.h) > EPS) {
        term = (term * t) / (n + 1);
        s = s + term;
        n++;
    }
    //вычислив e^t остаётся вычислить e^(t*k), e^t=s+1, подставим e^(t*k)=(s+1)^k
    //т.к. k - степень двойки, то рассмотрим следующий алгоритм возведения в степень
    //(s+1)^(2^p)=((((s+1)^2)^2)^...) - пока не будет возведено в квадрат p раз (быстрое возведение в степень)
    //введём последовательность: s с индексом 0 = s, s с индексом i+1 = 2*s с индексом i + (s с индексом i)^2, таким образом (1+s с индксом i)^2 = 1 + 2*s с индексом i + (s с индексом i)^2
    //после p итераций s с индком p = (s+1)^(2^p)-1=e^(k*t)-1
    for (int i = 0; i < p; ++i) {
        s = s * 2 + square_dd(s);
    }
    dd er = s + 1; //er = e^r

    dd rez = ldexp_dd(er, m);
    return rez;
}

dd pow_dd(const dd &x, const dd &y) {
    //вычисление проводятся по формуле pow(x, y)=exp(y*ln(x))
    if (x.h == 1.0 && x.t == 0.0) return dd(1.0);
    if (y.h == 0.0 && y.t == 0.0) return dd(1.0);
    if (y.h == 1.0 && y.t == 0.0) return x;
    if (x.h == 0.0 && x.t == 0.0) {
        if (y.h > 0.0) return dd(0.0);
        if (y.h == 0.0) return dd(1.0);
        return dd(INFINITY);
    }

    return exp_dd(y * ln_dd(x));
}

#endif //DD_POW_UTILS_H























