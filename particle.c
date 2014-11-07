#include "vector.h"

#define sq(x) ((x)*(x))
#define every(i) for (struct paricle* i = fluid; i != &fluid[N]; i++)

enum {
    N = 1000
};

const double D = 1;
const double Dm = 10 * D;
const double T = 0.001;
const struct vector G = {0, -10, 0};

struct particle {
    struct vector p, v, a;
};

struct plane {
    double A, B, C, D;
}

struct particle fluid[N] = {{0}};

void clear() {
    every (i)
        vec_clear(&i->a);
}

void interact() {
    every (i) {
        every (j) {
            if (i != j) {
                struct vector a = j->p;
                vec_sub(&a, &i->p);
                double d = vec_len(&a);
                double l = 1.0 / sq(d);
                if (d < D) {
                    vec_set_len(&a, -l);
                }
                else if (d >= D && d < Dm) {
                    vec_set_len(&a, l);
                }
                else {
                    vec_clear(&a);
                }
                vec_add(&i->a, &a);
            }
        }
    }
}

void gravity() {
    every (i)
        vec_add(&i->a, G);
}

void container(struct plane* p) {
    every (i) {
        double lam = (p->A * i->p.x + p->B * i->p.y + p->C * i->p.z + p->D)
            / (sq(p->A) + sq(p->B) + sq(p->C));
        struct vector norm = {p->A, p->B, p->C};
        vec_smul(&norm, lam);
        double d = vec_len(&norm);
        if (d < D) {
            vec_set_len(&norm, 1.0 / sq(d));
            vec_add(&i->a, &norm);
        }
    }
}

void update() {
    for (struct particle* i = fluid; i != &fluid[N]; i++) {
        struct vector s = i->v;
        vec_smul(&s, T);
        struct vector s2 = i->a;
        vec_add(&s, vec_smul(&s2, sq(T) * 0.5));
        struct vector dv = i->a;
        vec_smul(&dv, T);
        vec_add(&i->p, &s);
        vec_add(&i->v, &dv);
    }
}

void step() {
    clear();
    interact();
    gravity();
    container({0, 0, 1, 0});
    update();
}

void init() {
    for (int i = 0; i != N; i++) {
        vec_clear(&fluid[i].p);
        vec_clear(&fluid[i].v);
        vec_clear(&fluid[i].a);
        fluid[i].p.x = i % 200;
        fluid[i].p.y = i / 200;
    }
}
