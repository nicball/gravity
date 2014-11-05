#include "vector.h"

enum {
    N = 1000
};

const double D = 0.5 / N;
const double T = 0.001;

struct particle {
    struct vector p, v, a;
};

struct particle fluid[N] = {{0}};

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
                    i->a = *vec_set_len(&v, a);
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
        vec_clear(&fluid[i].p);
        vec_clear(&fluid[i].v);
        vec_clear(&fluid[i].a);
        fluid[i].p.x = i * 1.0 / N;
    }
}
