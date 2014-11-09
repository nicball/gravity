#include <GL/gl.h>
#include <GL/glut.h>
#include <stdio.h>
#include "vector.h"

#define sq(x) ((x)*(x))
#define qq(x) ((x)*(x)*(x)*(x))
#define hq(x) ((x)*(x)*(x)*(x)*(x)*(x))
#define every(i) for (struct particle* i = fluid; i != &fluid[N]; i++)

enum {
    N = 1000,
    CHECK_POINT = 100
};

const double K = 0.2;
const double ETA = 0.5;
const double D = 1;
const double Dm = 100 * D;
const double T = 0.05;
const struct vector G = {0, -10, 0};

double view_factor = 1.0 / 600.0;

struct particle {
    struct vector p, v, a;
};

struct plane {
    struct vector n;
    double C;
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
                if (d < D) {
                    vec_set_len(&a, -K / d);
                }
                else if (d >= D && d < Dm) {
                    vec_set_len(&a, K / hq(d));
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
        double lam = (vec_dot(&p->n, &i->p) + p->C) / vec_dot(&p->n, &p->n);
        struct vector norm = p->n;
        vec_smul(&norm, lam);
        double d = vec_len(&norm);
        if (d < 50 * D) {
            vec_set_len(&norm, 5 * (50 * D - d));
            //printf("plane=%lfx+%lfy+%lfz+%lf=0\n", p->n.x, p->n.y, p->n.z, p->C);
            //printf("a=(%lf, %lf, %lf) %lf\n", norm.x, norm.y, norm.z, vec_len(&norm));
            if (vec_dot(&norm, &i->v) / (vec_len(&norm) * vec_len(&i->v)) > 0)
                vec_smul(&norm, ETA);
            vec_add(&i->a, &norm);
        }
    }
}

void update() {
    every (i) {
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
    container(&(struct plane){{0, 1, 0}, 400});
    update();
}

void init() {
    for (int i = 0; i != N; i++) {
        vec_clear(&fluid[i].p);
        vec_clear(&fluid[i].v);
        vec_clear(&fluid[i].a);
        //fluid[i].p.x = i % 10 * 1.02;
        //fluid[i].p.y = i / 100 * 1.02 + 100;
        //fluid[i].p.z = i % 100 / 10 * 1.02;
        fluid[i].p.x = i % 30 * 1.2;
        fluid[i].p.y = i / 30 * 1.2 + 100;
    }
    printstats();
}

void render() {
    //for (int i = 0; i < CHECK_POINT; i++)
        step();
    printstats();

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0, 0.7);
    glBegin(GL_POINTS);
    every (i)
        glVertex3d(i->p.x * view_factor, i->p.y * view_factor, i->p.z * view_factor);
    glEnd();
    glBegin(GL_LINES);
    glVertex3d(-1, -400 * view_factor, 0);
    glVertex3d(1, -400 * view_factor, 0);
    glEnd();
    glFlush();
    glutSwapBuffers();
}

void on_mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        view_factor *= 2.0;
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        view_factor /= 2.0;
    }
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
    glutMouseFunc(on_mouse);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glutMainLoop();
    return 0;
}
