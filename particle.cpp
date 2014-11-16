#include "particle.hpp"

particle::list& particle::list::interact() {
    for (auto& i : *this) {
        for (auto& j : *this) {
            if (&i != &j) {
                auto a = j.position;
                a -= i.position;
                auto d = a.length();
                if (d == 0) {
                    continue;
                }
                else if (d < D) {
                    a.set_length(-K / d);
                }
                else if (d >= D && d < Dm) {
                    a.set_length(K / (d*d*d*d*d*d));
                }
                else {
                    a.clear();
                }
                i.acceleration += a;
            }
        }
    }
    return *this;
}
