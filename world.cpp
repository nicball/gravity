#include "world.h"

void interact(world& w) {
    static constexpr double R = 0.2;
    static constexpr double K = 0.5;
    static constexpr double E = 0.001;
    for (auto& i : w) {
        for (auto& j : w) {
            if (&i != &j) {
                auto a = j.position - i.position;
                auto d = a.length();
                auto f = E*i.mass*j.mass * (1 / (d*d) - 1 / (d*d*d));
                if (d < R) f += (R - d) * K;
                a.set_length(f);
                i.acceleration += a;
            }
        }
    }
}
