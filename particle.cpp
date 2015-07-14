#include "particle.hpp"

particle::list& particle::list::interact() {
    for (auto& i : *this) {
        for (auto& j : *this) {
            if (&i != &j) {
                auto a = j.position;
                a -= i.position;
                auto d = a.length();
                if (d < Dmin || d > 100*d) continue;
                a.set_length((K/(d*d) - K/(d*d*d*d*d*d)) / 100);
                i.acceleration += a;
            }
        }
    }
    return *this;
}
