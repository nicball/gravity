#include <stdio.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include "vector.h"

const double G = 6.6732E-11;
const double ACCURACY = 1;
const int CHECK_POINT = 60 * 60;
double view_factor = 1.0;
double view_x = 0.0;
double view_y = 0.0;

typedef struct object object;

struct object {
    const char* name;
    vector position;
    vector velocity;
    double mass;
    double radius;
    float color[3];
};

object world[] = {
    { "sun", { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, 1.989E30, 6.96342E8, { 1.0f, 1.0f, 0.0f } },
    { "mecury", { 0.0, 4.60012E10, 0.0 }, { 4.787E4, 0.0, 0.0 }, 3.3022E23, 2.4397E6, { 0.8f, 0.8f, 0.8f } },
    { "venus", { 0.0, 1.07477E11, 0.0 }, { 3.502E4, 0.0, 0.0 }, 4.8676E24, 6.0518E6, { 0.8f, 1.0f, 0.0f } },
    { "earth", { 0.0, 1.47098074E11, 0.0 }, { 2.979E4, 0.0, 0.0 }, 5.9742E24, 6.371E6, { 0.0f, 1.0f, 1.0f } },
        { "moon", { 0.0, 1.47461178E11, 0.0 }, { 3.0812E4, 0.0, 0.0 }, 7.3477E22, 1.7371E6, { 1.0f, 1.0f, 1.0f } },
    { "mars", { 0.0, 2.0622E11, 0.0 }, { 2.413E4, 0.0, 0.0 }, 6.4185E23, 3.3895E6, { 0.6f, 0.3f, 0.2f } },
    { "jupiter", { 0.0, 7.405736E11, 0.0 }, { 1.307E4, 0.0, 0.0 }, 1.8986E27, 6.9911E7, { 0.8f, 0.2f, 0.0f } },
    { "saturn", { 0.0, 1.353572956E12, 0.0 }, { 9.69E3, 0.0, 0.0 }, 5.6846E26, 5.8232E7, { 0.8f, 0.6f, 0.0f } },
    { "uranus", { 0.0, 2.748938461E12, 0.0 }, { 6.81E3, 0.0, 0.0, }, 8.6810E25, 2.5362E7, { 0.4f, 0.8f, 1.0f } },
    { "neptune", { 0.0, 4.452940833E12, 0.0 }, { 5.43E3, 0.0, 0.0 }, 1.0243E26, 2.4622E7, { 0.2f, 0.6f, 1.0f } },
    { "VY Canis Majoris", { -5E12, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, 1.989E30 * 17, 6.96342E8 * 1420, { 1.0f, 0.0f, 0.0f } }
};

const int NOBJ = sizeof world / sizeof world[0];

vector accel(object* forced, object* exert) {
    vector dir = vec_sub(exert->position, forced->position);
    double L = vec_len(dir);
    double a = G * exert->mass / ( L * L );
    return vec_with_len(dir, a);
}

object update(object obj, vector a) {
    vector v = vec_add(obj.velocity, vec_smul(a, ACCURACY));
    vector dx = vec_add(vec_smul(obj.velocity, ACCURACY),
                        vec_smul(a, ACCURACY * ACCURACY * 0.5));
    obj.position = vec_add(obj.position, dx);
    obj.velocity = v;
    return obj;
}

void print(object* o) {
    printf("%s\n\
            \tposition: (%G, %G, %G)\n\
            \tvelocity: %G m/s (%G, %G, %G)\n",
            o->name,
            o->position.x, o->position.y, o->position.z,
            vec_len(o->velocity), o->velocity.x, o->velocity.y, o->velocity.z);
}

void step(object world[], int NOBJ) {
        object new_world[NOBJ];
        object* p = new_world;
        for (object* obj = world; obj - world != NOBJ; obj++) {
            vector a = { 0.0, 0.0, 0.0 };
            for (object* i = world; i - world != NOBJ; i++) {
                if (i != obj) {
                    a = vec_add(a, accel(obj, i));
                }
            }
            *p++ = update(*obj, a);
        }
        memcpy(world, new_world, sizeof world[0] * NOBJ);
}

void draw_circle(double x, double y, double radius) {
    glVertex2d(x, y);
    for (int i = 0; i <= 100; i++) {
        double angle = i * 2 * 3.1415926 / 100;
        glVertex2d(x + radius * cos(angle), y + radius * sin(angle));
    }
}

void render() {
    for (int i = 0; i < CHECK_POINT; i++)
        step(world, NOBJ);
//    for (object* o = world; o - world != NOBJ; o++)
//        print(o);

    glClear(GL_COLOR_BUFFER_BIT);
    for (object* o = world; o - world != NOBJ; o++) {
//        glBegin(GL_TRIANGLE_FAN);
        glBegin(GL_POINTS);
        glColor3fv(o->color);
//        draw_circle(o->position.x * view_factor / 8e11,
//                    o->position.y * view_factor / 8e11,
//                    o->radius * view_factor / 8e11);
        glVertex2d(o->position.x * view_factor / 8e11 + view_x,
                   o->position.y * view_factor / 8e11 + view_y);
        glEnd();
        glFlush();
    }
    glutSwapBuffers();
}

void on_mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        view_factor *= 2.0;
        view_x *= 2.0;
        view_y *= 2.0;
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        view_factor /= 2.0;
        view_x /= 2.0;
        view_y /= 2.0;
    }
}

void on_keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            view_x += 0.1;
            break;

        case GLUT_KEY_RIGHT:
            view_x -= 0.1;
            break;

        case GLUT_KEY_UP:
            view_y -= 0.1;
            break;

        case GLUT_KEY_DOWN:
            view_y += 0.1;
            break;

        default:
            puts("Unknown key press.");
            break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(1440, 900);
    glutCreateWindow("");
    glutDisplayFunc(render);
    glutMouseFunc(on_mouse);
    glutKeyboardFunc(on_keyboard);
    glutIdleFunc(render);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glutMainLoop();
    return 0;
}
