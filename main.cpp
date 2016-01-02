#ifdef _WIN32
#include <Windows.h>
#endif
#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>

#include "world.h"
#include "plane.h"

class world world(1000);
double view_factor = 1.0 / 1600.0;
constexpr double WATER_MASS = 100;
constexpr double AIR_MASS = 1;

void init() {
    for (int i = 0; i != world.size(); ++i) {
        if (i % 2 == 0) {
            world[i].mass = AIR_MASS;
        }
        else {
            world[i].mass = WATER_MASS;
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
        .add_npc(interact)
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

void render() {
    static double counter = 0.0;
    world.step();
    counter += world::T;
    if (std::floor(counter) == 1.0) {
        std::cout << "PIA" << std::endl;
        counter = 0.0;
    }

    static constexpr float BLUE[] = {0.0f, 0.0f, 0.7f};
    static constexpr float RED[] = {0.7f, 0.0f, 0.0f};
    static constexpr float WHITE[] = {1.0f, 1.0f, 1.0f};
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);
    for (auto& i : world) {
        if (i.mass == WATER_MASS) {
            glColor3fv(BLUE);
        }
        else if (i.mass == AIR_MASS) {
            glColor3fv(RED);
        }
        else {
            glColor3fv(WHITE);
        }
        glVertex3d(i.position.x * view_factor, i.position.y * view_factor, i.position.z * view_factor);
    }
    glEnd();

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
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("");
    glutDisplayFunc(render);
    glutIdleFunc(render);
    glutMouseFunc(on_mouse);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glutMainLoop();
    return 0;
}
