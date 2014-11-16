#include "gravity.hpp"

void gravity(particle::list& arr) {
    for (auto& i : arr)
        i.acceleration += G;
}
