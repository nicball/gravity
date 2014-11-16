#ifndef GRAVITY_HPP_
#define GRAVITY_HPP_

#include "particle.hpp"

constexpr vector3d G = {0, -10, 0};
void gravity(particle::list& arr);

#endif
