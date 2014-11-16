#include "plane.hpp"

void plane::act_on(particle::list& arr) const {
    for (auto& i : arr) {
        auto norm = normal;
        auto lam = (norm*i.position + C) / (norm*norm);
        norm *= lam;
        double d = norm.length();
        if (d < D) {
            auto r = std::tan(frand()*MU) * norm.length();
            auto ytoz = std::atan2(norm.y, norm.z);
            auto ztox = std::atan2(norm.z, norm.x) - 0.5*PI;
            vector3d dn;
            dn.x = rs() * frand() * r;
            dn.y = rs() * std::sqrt(r*r - dn.x*dn.x);
            dn.z = 0;
            dn.rotate_x(-ytoz);
            dn.rotate_y(ztox);
            norm += dn;
            norm.set_length(K * (D-d));
            if (norm*i.velocity / (norm.length()*i.velocity.length()) > 0)
                norm *= ETA;
            i.acceleration += norm;
        }
    }
}

