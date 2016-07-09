#ifdef _WIN32
#include <Windows.h>
#endif
#include <GL/gl.h>
#include <GL/glut.h>
#include <algorithm>
#include <iostream>

#include "world.h"
#include "plane.h"

template <class T, size_t N>
void array_assign(const T (&from)[N], T (&to)[N]) {
    std::copy(std::begin(from), std::end(from), std::begin(to));
}

constexpr size_t WORLD_SIZE = 10;
class world world(WORLD_SIZE);
double view_factor = 1.0;
double view_x = 0;
double view_y = 0;
constexpr double WATER_MASS = 100;
constexpr double AIR_MASS = 1;
constexpr float BLUE[] = {0.0f, 0.0f, 0.7f};
constexpr float RED[] = {0.7f, 0.0f, 0.0f};
constexpr float WHITE[] = {1.0f, 1.0f, 1.0f};

void init_water_simulation() {
    view_factor = 1.0 / 1600.0;
    for (int i = 0; i != WORLD_SIZE; ++i) {
        if (i % 2 == 0) {
            world[i].mass = AIR_MASS;
            array_assign(RED, world[i].color);
        }
        else {
            world[i].mass = WATER_MASS;
            array_assign(BLUE, world[i].color);
        }
        //world[i].position.x = (i%10-5) * 1.0;
        //world[i].position.y = i/100*1.0 + 100;
        //world[i].position.z = i%100/10 * 1.0;

        //world[i].position.x = (i%30-15) * 4;
        //world[i].position.y = i/30*4;

        world[i].position.y = 4*i + 500;
        world[i].position.x = 0;

        //world[i].position.y = 4*i+500;
        //world[i].position.x = 400;
    }
    world.add_npc(gravity)
        .add_npc([](auto&& x) { interact(x); })
        //.add_npc(plane{{0, 1, 0}, 400})
        //.add_npc(plane{{0, 0, 1}, -100})
        //.add_npc(plane{{0, 0, 1}, 100})
        //.add_npc(plane{{1, 1, 0}, 0})
        //.add_npc(plane{{1, -1, 0}, 0});

        .add_npc(plane{{0, 1, 0}, 100})
        .add_npc(plane{{1, 0, 0}, -60})
        .add_npc(plane{{-1, 0, 0}, -60})
        .add_npc(plane{{0, 0, 1}, -60})
        .add_npc(plane{{0, 0, -1}, -60})
        ;
}

void init_solar_system_simulation() {
    constexpr double G = 6.6732E-11;
    view_factor = 1.0 / 8e11;
    particle data[] = {
        { "sun", { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, 1.989E30, 6.96342E8, { 1.0f, 1.0f, 0.0f }, {} },
        { "mecury", { 0.0, 4.60012E10, 0.0 }, { 4.787E4, 0.0, 0.0 }, 3.3022E23, 2.4397E6, { 0.8f, 0.8f, 0.8f }, {} },
        { "venus", { 0.0, 1.07477E11, 0.0 }, { 3.502E4, 0.0, 0.0 }, 4.8676E24, 6.0518E6, { 0.8f, 1.0f, 0.0f }, {} },
        { "earth", { 0.0, 1.47098074E11, 0.0 }, { 2.979E4, 0.0, 0.0 }, 5.9742E24, 6.371E6, { 0.0f, 1.0f, 1.0f }, {} },
            { "moon", { 0.0, 1.47461178E11, 0.0 }, { 3.0812E4, 0.0, 0.0 }, 7.3477E22, 1.7371E6, { 1.0f, 1.0f, 1.0f }, {} },
        { "mars", { 0.0, 2.0622E11, 0.0 }, { 2.413E4, 0.0, 0.0 }, 6.4185E23, 3.3895E6, { 0.6f, 0.3f, 0.2f }, {} },
        { "jupiter", { 0.0, 7.405736E11, 0.0 }, { 1.307E4, 0.0, 0.0 }, 1.8986E27, 6.9911E7, { 0.8f, 0.2f, 0.0f }, {} },
        { "saturn", { 0.0, 1.353572956E12, 0.0 }, { 9.69E3, 0.0, 0.0 }, 5.6846E26, 5.8232E7, { 0.8f, 0.6f, 0.0f }, {} },
        { "uranus", { 0.0, 2.748938461E12, 0.0 }, { 6.81E3, 0.0, 0.0, }, 8.6810E25, 2.5362E7, { 0.4f, 0.8f, 1.0f }, {} },
        { "neptune", { 0.0, 4.452940833E12, 0.0 }, { 5.43E3, 0.0, 0.0 }, 1.0243E26, 2.4622E7, { 0.2f, 0.6f, 1.0f }, {} },

        //{ "VY Canis Majoris", { -5E12, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, 1.989E30 * 17, 6.96342E8 * 1420, { 1.0f, 0.0f, 0.0f }, {} }
    };
    for (int i = 0; i != WORLD_SIZE; ++i) {
        world[i] = data[i];
    }
    world.add_npc([](auto&& x) { interact(x, G); });
}

void render() {
    for (auto& i : world) {
        std::cout << i.name
                  << " v="
                  << i.velocity
                  << " a="
                  << i.acceleration
                  << std::endl;
    }
    std::cout << std::endl;
    
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);
    for (auto& i : world) {
        glColor3fv(i.color);
        glVertex3d(i.position.x * view_factor + view_x, i.position.y * view_factor + view_y, i.position.z * view_factor);
    }
    glEnd();
/*
    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
    glVertex3d(-1, -100 * view_factor, 0);
    glVertex3d(1, -100 * view_factor, 0);
    glEnd();

    glBegin(GL_LINES);
    glVertex3d(60 * view_factor, 1, 0);
    glVertex3d(60 * view_factor, -1, 0);
    glEnd();

    glBegin(GL_LINES);
    glVertex3d(-60 * view_factor, 1, 0);
    glVertex3d(-60 * view_factor, -1, 0);
    glEnd();
*/
/*
    glBegin(GL_LINES);
    glVertex3d(-1, 1, 0);
    glVertex3d(1, -1, 0);
    glEnd();

    glBegin(GL_LINES);
    glVertex3d(-1, -1, 0);
    glVertex3d(1, 1, 0);
    glEnd();
*/
    glFlush();
    glutSwapBuffers();

    world.step();
    static double counter = 0.0, sum = 0.0;
    counter += world::T;
    sum += world::T;
    if (std::floor(counter) == 1.0) {
        std::cout << "PIA "  << sum << std::endl;
        counter = 0.0;
    }
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

void on_specialkey(int key, int x, int y) {
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
    init_solar_system_simulation();
    // init_water_simulation();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("");
    glutDisplayFunc(render);
    glutSpecialFunc(on_specialkey);
    glutIdleFunc(render);
    glutMouseFunc(on_mouse);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glutMainLoop();
    return 0;
}
