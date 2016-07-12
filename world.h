#ifndef PARTICLE_H
#define PARTICLE_H
#pragma once

#include "vector3d.h"
#include <array>
#include <functional>
#include <vector>
#include <iostream>

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

inline std::ostream& operator<< (std::ostream& os, const particle& p) {
    os << p.name
       << " pos=" << p.position
       << " v=" << p.velocity
       << " m=" << p.mass
       << " r=" << p.radius
       << " a=" << p.acceleration;
    return os;
}

class world: private std::vector<particle> {
    double T = 0.05;
public:

    world(size_t n): vector<particle>(n) {}
    world(size_t n, double acc): vector<particle>(n), T{acc} {}
    ~world() = default;

    using action = std::function<void(world&)>;

    using std::vector<particle>::begin;
    using std::vector<particle>::end;
    using std::vector<particle>::operator[];
    using std::vector<particle>::size;

    double accuracy() { return T; }
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

void interact(class world&, double E = 0.001, double K = 0.2);

inline void gravity(class world& w) {
    constexpr vector3d G = {0, -10, 0};
    for (auto& i : w) {
        i.acceleration += G;
    }
}

#endif
