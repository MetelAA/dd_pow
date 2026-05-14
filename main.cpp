#include <iostream>
#include "dd.h"
#include "utils.h"

dd create_dd(double h, double t){
    double head, tail;
    head = e_math::add2(h, t, tail);
    return {head, tail};
}

int main(int argc, char* argv[]) {
    if(argc != 5 && argc != 9){
        std::cerr << "You can enter two values: -xh x.head -yh y.head or four values: -xh x.head -xt x.tail -yh y.head -yt y.tail" << std::endl;
        return 1;
    }

    double x_head = 0.0, x_tail = 0.0;
    double y_head = 0.0, y_tail = 0.0;
    bool x_has_tail = false, y_has_tail = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-xh" && i+1 < argc) {
            x_head = std::stod(argv[++i]);
            x_has_tail = true;
        } else if (arg == "-xt" && i+1 < argc) {
            x_tail = std::stod(argv[++i]);
        } else if (arg == "-yh" && i+1 < argc) {
            y_head = std::stod(argv[++i]);
            y_has_tail = true;
        } else if (arg == "-yt" && i+1 < argc) {
            y_tail = std::stod(argv[++i]);
        } else {
            std::cerr << "Unknown argument or missing value: " << arg << std::endl;
            return 1;
        }
    }

    if (!x_has_tail || !y_has_tail) {
        std::cerr << "Error: both -xh and -yh are required." << std::endl;
        return 1;
    }

    dd x = create_dd(x_head, x_tail);
    dd y = create_dd(y_head, y_tail);

    if(x.h < 0){
        std::cerr << "x must be more then 0" << std::endl;
        return 1;
    }

    dd rez = pow_dd(x, y);
    std::cout << to_string_dd(rez) << std::endl;


    return 0;
}
