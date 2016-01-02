#ifndef PARTICLE_H
#define PARTICLE_H
#pragma once

#include "vector3d.h"
#include <array>
#include <functional>
#include <vector>

struct particle {
    double mass;
    vector3d position;
    vector3d velocity;
    vector3d acceleration;

    particle(): mass{}, position{}, velocity{}, acceleration{} {}
};

class world: public std::vector<particle> {
public:
    static constexpr double T = 0.05;

    world(size_t n): vector<particle>(n) {}
    ~world() = default;

    using action = std::function<void(world&)>;

    world& add_npc(action a) {
        npc.push_back(a);
        return *this;
    }

    world& clear_npc() {
        npc.clear();
        return *this;
    }

    world& step() {
        for (auto& i : *this)
            i.acceleration.clear();
        for (auto& n : npc)
            n(*this);
        update();
        return *this;
    }

    world& update() {
        for (auto& i : *this) {
            vector3d s = i.velocity;
            s *= T;
            vector3d s2 = i.acceleration;
            s2 *= T * T * 0.5;
            s += s2;
            vector3d dv = i.acceleration;
            dv *= T;
            i.position += s;
            i.velocity += dv;
        }
        return *this;
    }

private:
    std::vector<action> npc;
};

void interact(class world&);

constexpr vector3d G = {0, -10, 0};
inline void gravity(class world& w) {
    for (auto& i : w) {
        i.acceleration += G;
    }
}

#endif
