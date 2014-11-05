#include "vector.h"

enum {
    N = 1000
};

const double D = 0.5 / N;
const double T = 0.001;

struct particle {
    struct vector p, v, a;
};

struct particle fluid[N];

void interact() {
    for (struct particle* i = fluid; i != &fluid[N]; i++) {
        for (struct particle* j = fluid; j != &fluid[N]; j++) {
            if (i != j) {
                struct vector v = j->p;
                vec_sub(&v, &i->p);
                double d = vec_len(&v);
                double a = 1.0 / (d * d);
                if (d < D) {
                    vec_clear(&i->a);
                    vec_set_len(vec_sub(&i->a, &v), a);
                }
                else if (d >= D) {
                    i->a = *vec_set_len(&v, d);
                }
            }
        }
    }
}

void update() {
    for (struct particle* i = fluid; i != &fluid[N]; i++) {
        struct vector s = i->v;
        vec_smul(&s, T);
        struct vector s2 = i->a;
        vec_add(&s, vec_smul(&s2, T * T * 0.5));
        struct dv = i->a;
        vec_smul(&dv, T);
        vec_add(&i->p, &s);
        vec_add(&i->v, &dv);
    }
}

void step() {
    interact();
    update();
}

void init() {
    for (int i = 0; i != N; i++) {
        fluid[i].p.x = i * 1.0 / N;
        fluid[i].p.y = fluid[i].p.z
            = fluid[i].v.x = fluid[i].v.y = fluid[i].v.z
            = fluid[i].a.x = fluid[i].a.y = fluid[i].a.z
            = 0;
    }
}
