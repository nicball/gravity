#include "plane.h"

void plane::act_on(world& w) const {
    for (auto& i : w) {
        auto pos = normal * (normal*i.position + C) / (normal*normal);
        double d = pos.length();
        if (d < D) {
            auto r = std::tan(frand()*MU) * pos.length();
            auto ytoz = std::atan2(pos.z, pos.y);
            auto xtoz = std::atan2(pos.z, pos.x);
            vector3d dn;
            dn.x = rs() * frand() * r;
            dn.y = rs() * std::sqrt(r*r - dn.x*dn.x);
            dn.z = 0;
            dn.rotate_x(-ytoz);
            dn.rotate_y(-xtoz);
            auto a = pos + dn;
            a.set_length(K * (D - d));
            if (a*i.velocity / (a.length()*i.velocity.length()) > 0)
                a *= ETA;
            i.acceleration += a;
        }
    }
}
