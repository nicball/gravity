#ifndef PLANE_H
#define PLANE_H
#pragma once

#include "vector3d.h"
#include "world.h"
#include <random>

struct plane {
    static constexpr double K = 500;
    static constexpr double ETA = 0.3;
    static constexpr double MU = 0.25 * PI;
    static constexpr double D = 50;

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

    void act_on(world& arr) const;
    void operator () (world& arr) const { act_on(arr); }
};

#endif
