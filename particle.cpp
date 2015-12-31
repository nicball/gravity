#include "world.h"

void interact(world& w) {
    for (auto& i : w) {
        for (auto& j : w) {
            if (&i != &j) {
                auto a = j.position - i.position;
                auto d = a.length();
                if (d < world::Dmin || d > 100 * world::Dmin) continue;
                a.set_length(world::K / (d*d*d) - world::K / (d*d));
                i.acceleration += a;
            }
        }
    }
}
