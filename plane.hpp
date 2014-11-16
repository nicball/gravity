#ifndef PLANE_HPP_
#define PLANE_HPP_

#include <random>
#include "particle.hpp"

struct plane {
    static constexpr double PI  = particle::PI;
    static constexpr double K   = 40;
    static constexpr double ETA = 0.3;
    static constexpr double MU  = 0.25 * PI;
    static constexpr double D   = 50 * particle::D;

    vector3d normal;
    double C;

    static double frand() {
        static std::mt19937 gen{std::random_device{}()};
        static std::uniform_real_distribution<> dis{0, 1};
        return dis(gen);
    }

    static int rs() {
        return frand()>0.5 ? 1 : -1;
    }

    void act_on(particle::list& arr) const;
    void operator () (particle::list& arr) const { act_on(arr); }
};

#endif
