#ifndef PARTICLE_HPP_
#define PARTICLE_HPP_

#include <array>
#include <functional>
#include <vector>
#include "vector3d.hpp"

struct particle {
    class list;
    static constexpr int N      = 1000;
    static constexpr double PI  = 3.1415927;
    static constexpr double T   = 0.05;
    static constexpr double D   = 1;
    static constexpr double Dm  = 100 * D;
    static constexpr double K   = 0.2;

    vector3d position;
    vector3d velocity;
    vector3d acceleration;
};

class particle::list: public std::array<particle, N> {
public:
    using action = std::function<void(list&)>;

    list& add_npc(action a) {
        npc.push_back(a);
        return *this;
    }

    list& clear_npc() {
        npc.clear();
        return *this;
    }

    list& step() {
        for (auto& i : *this)
            i.acceleration.clear();
        for (auto& n : npc)
            n(*this);
        interact();
        update();
        return *this;
    }

    list& update() {
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

    list& interact();

private:
    std::vector<action> npc;
};

#endif
