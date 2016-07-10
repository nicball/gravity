#include "world.h"

void interact(world& w, double E) {
    static constexpr double K = 0.5;
    for (auto& i : w) {
        for (auto& j : w) {
            if (&i != &j) {
                auto a = j.position - i.position;
                auto d = a.length();
                auto f = E*j.mass * (1 / (d*d) - 1 / (d*d*d));
                auto R = i.radius + j.radius;
                if (d < R) f += (R - d) * K;
                a.set_length(f);
                i.acceleration += a;
            }
        }
    }
}
