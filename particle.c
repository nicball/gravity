#include <GL/gl.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "vector.h"

#define rs() (rand()%2 ? 1 : -1)
#define frand() (rand() * 1.0 / RAND_MAX)
#define sq(x) ((x)*(x))
#define qq(x) ((x)*(x)*(x)*(x))
#define hq(x) ((x)*(x)*(x)*(x)*(x)*(x))

#define N    1000
#define PI   3.1415927
#define Km   0.2
#define Kn   40
#define ETA  0.3
#define MU   (0.25 * PI)
#define D    1
#define Dm   (100 * D)
#define Dw   (50 * D)
#define T    0.05
const struct vector G = {0, -10, 0};

double view_factor = 1.0 / 1600.0;

struct particle {
    struct vector position;
    struct vector velocity;
    struct vector acceleration;
};

struct plane {
    struct vector normal;
    double C;
};

struct particle fluid[N];

void clear() {
    for (struct particle* i = fluid; i != &fluid[N]; i++)
        vec_clear(&i->acceleration);
}

void interact() {
    for (struct particle* i = fluid; i != &fluid[N]; i++) {
        for (struct particle* j = fluid; j != &fluid[N]; j++) {
            if (i != j) {
                struct vector a = j->position;
                vec_sub(&a, &i->position);
                double d = vec_len(&a);
                if (d == 0) {
                    continue;
                }
                else if (d < D) {
                    vec_set_len(&a, -Km / d);
                }
                else if (d >= D && d < Dm) {
                    vec_set_len(&a, Km / hq(d));
                }
                else {
                    vec_clear(&a);
                }
                vec_add(&i->acceleration, &a);
            }
        }
    }
}

void gravity() {
    for (struct particle* i = fluid; i != &fluid[N]; i++)
        vec_add(&i->acceleration, &G);
}

void container(const struct plane* planes, int n) {
    for (struct particle* i = fluid; i != &fluid[N]; i++) {
        for (const struct plane* p = planes; p != &planes[n]; p++) {
            struct vector norm = p->normal;
            double lam = (vec_dot(&norm, &i->position) + p->C) / vec_dot(&norm, &norm);
            vec_smul(&norm, lam);
            double d = vec_len(&norm);
            if (d < Dw) {
                double r = tan(frand()*MU) * vec_len(&norm);
                double ytoz = atan2(norm.y, norm.z);
                double ztox = atan2(norm.z, norm.x) - 0.5*PI;
                struct vector dn;
                dn.x = rs() * frand() * r;
                dn.y = rs() * sqrt(sq(r)-sq(dn.x));
                dn.z = 0;
                vec_rotate_x(&dn, -ytoz);
                vec_rotate_y(&dn, ztox);
                vec_add(&norm, &dn);
                vec_set_len(&norm, Kn * (Dw-d));
                if (vec_dot(&norm, &i->velocity) / (vec_len(&norm)*vec_len(&i->velocity)) > 0)
                    vec_smul(&norm, ETA);
                vec_add(&i->acceleration, &norm);
            }
        }
    }
}

void update() {
    for (struct particle* i = fluid; i != &fluid[N]; i++) {
        struct vector s = i->velocity;
        vec_smul(&s, T);
        struct vector s2 = i->acceleration;
        vec_add(&s, vec_smul(&s2, sq(T) * 0.5));
        struct vector dv = i->acceleration;
        vec_smul(&dv, T);
        vec_add(&i->position, &s);
        vec_add(&i->velocity, &dv);
    }
}

void step() {
    static const struct plane walls[] = {
        {{0, 1, 0},  400},
        {{0, 0, 1},  -100},
        {{0, 0, 1},  100},
        {{1, 0, 0},  -100},
        {{1, 0, 0},  100}
    };

    clear();
    interact();
    gravity();
    container(walls, sizeof(walls) / sizeof(walls[0]));
    update();
}

void init() {
    for (int i = 0; i != N; i++) {
        vec_clear(&fluid[i].position);
        vec_clear(&fluid[i].velocity);
        vec_clear(&fluid[i].acceleration);
        //fluid[i].position.x = (i%10-5) * 1.0;
        //fluid[i].position.y = i/100*1.0 + 100;
        //fluid[i].position.z = i%100/10 * 1.0;

        //fluid[i].position.x = (i%30-15) * 1.2;
        //fluid[i].position.y = i/30*1.2 + 300;
        
        fluid[i].position.y = i+800;
    }
}

void render() {
    step();

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0, 0.7);
    glBegin(GL_POINTS);
    for (struct particle* i = fluid; i != &fluid[N]; i++)
        glVertex3d(i->position.x * view_factor, i->position.y * view_factor, i->position.z * view_factor);
    glEnd();

    glBegin(GL_LINES);
    glVertex3d(-1, -400 * view_factor, 0);
    glVertex3d(1, -400 * view_factor, 0);
    glEnd();

    glBegin(GL_LINES);
    glVertex3d(100 * view_factor, 1, 0);
    glVertex3d(100 * view_factor, -1, 0);
    glEnd();

    glBegin(GL_LINES);
    glVertex3d(-100 * view_factor, 1, 0);
    glVertex3d(-100 * view_factor, -1, 0);
    glEnd();

    //glBegin(GL_LINES);
    //glVertex3d(-1, 1, 0);
    //glVertex3d(1, -1, 0);
    //glEnd();

    //glBegin(GL_LINES);
    //glVertex3d(-1, -1, 0);
    //glVertex3d(1, 1, 0);
    //glEnd();

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
    srand(time(0));
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
