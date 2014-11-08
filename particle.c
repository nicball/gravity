#include <GL/gl.h>
#include <GL/glut.h>
#include <stdio.h>
#include "vector.h"

#define sq(x) ((x)*(x))
#define every(i) for (struct particle* i = fluid; i != &fluid[N]; i++)

enum {
    N = 100,
    CHECK_POINT = 100
};

const double K = 1.0e-10;
const double D = 1;
const double Dm = 2 * D;
const double T = 0.01;
const struct vector G = {0, -10, 0};

struct particle {
    struct vector p, v, a;
};

struct plane {
    double A, B, C, D;
};

struct particle fluid[N];

void printstats() {
#ifdef VERBOSE
    printf("%dth particle: position=(%lf,%lf,%lf) velocity=(%lf,%lf,%lf)\n",
            1, fluid[1].p.x, fluid[1].p.y, fluid[1].p.z, fluid[1].v.x, fluid[1].v.y, fluid[1].v.z);
    printf("%dth particle: position=(%lf,%lf,%lf) velocity=(%lf,%lf,%lf)\n",
            250, fluid[250].p.x, fluid[250].p.y, fluid[250].p.z, fluid[250].v.x, fluid[250].v.y, fluid[250].v.z);
    printf("%dth particle: position=(%lf,%lf,%lf) velocity=(%lf,%lf,%lf)\n",
            500, fluid[500].p.x, fluid[500].p.y, fluid[500].p.z, fluid[500].v.x, fluid[500].v.y, fluid[500].v.z);
    printf("%dth particle: position=(%lf,%lf,%lf) velocity=(%lf,%lf,%lf)\n",
            999, fluid[999].p.x, fluid[999].p.y, fluid[999].p.z, fluid[999].v.x, fluid[999].v.y, fluid[999].v.z);
    putchar('\n');
#endif
}

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
                double l = K / sq(d);
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
        vec_add(&i->a, &G);
}

void container(struct plane* p) {
    every (i) {
        double lam = (p->A * i->p.x + p->B * i->p.y + p->C * i->p.z + p->D)
            / (sq(p->A) + sq(p->B) + sq(p->C));
        struct vector norm = {p->A, p->B, p->C};
        vec_smul(&norm, lam);
        double d = vec_len(&norm);
        if (d < D) {
            vec_set_len(&norm, 1 / d);
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
    container(&(struct plane){0, 1, 0, 10});
    update();
}

void init() {
    for (int i = 0; i != N; i++) {
        vec_clear(&fluid[i].p);
        vec_clear(&fluid[i].v);
        vec_clear(&fluid[i].a);
        fluid[i].p.x = i % 10 * 0.1;
        fluid[i].p.y = i / 10 * 0.1;
    }
    printstats();
}

void render() {
    //for (int i = 0; i < CHECK_POINT; i++)
        step();
    printstats();

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0, 0.7);
    every (i) {
        glBegin(GL_POINTS);
        glVertex3d(i->p.x/200, i->p.y/200, i->p.z/200);
        glEnd();
        glFlush();
    }
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    init();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(1440, 900);
    glutCreateWindow("");
    glutDisplayFunc(render);
    glutIdleFunc(render);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glutMainLoop();
    return 0;
}
