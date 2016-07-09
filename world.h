#ifndef PARTICLE_H
#define PARTICLE_H
#pragma once

#include "vector3d.h"
#include <array>
#include <functional>
#include <vector>

struct particle {
    std::string name;
    vector3d position;
    vector3d velocity;
    double mass{};
    double radius{};
    float color[3]{};
    vector3d acceleration;

    particle() = default;
};

class world: private std::vector<particle> {
public:
    static constexpr double T = 0.05;

    world(size_t n): vector<particle>(n) {}
    ~world() = default;

    using action = std::function<void(world&)>;

    using std::vector<particle>::begin;
    using std::vector<particle>::end;
    using std::vector<particle>::operator[];
    using std::vector<particle>::size;

    world& add_npc(action a) {
        npc.push_back(a);
        return *this;
    }

    world& clear_npc() {
        npc.clear();
        return *this;
    }

    world& step() {
        for (auto& n : npc)
            n(*this);
        update();
        for (auto& i : *this)
            i.acceleration.clear();
        return *this;
    }

    world& update() {
        for (auto& i : *this) {
            vector3d s = i.velocity * T;
            s += 0.5*i.acceleration*T*T;
            vector3d dv = i.acceleration * T;
            i.position += s;
            i.velocity += dv;
        }
        return *this;
    }

private:
    std::vector<action> npc;
};

void interact(class world&, double E = 0.001);

inline void gravity(class world& w) {
    constexpr vector3d G = {0, -10, 0};
    for (auto& i : w) {
        i.acceleration += G;
    }
}

#endif
