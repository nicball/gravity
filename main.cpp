#include <GL/gl.h>
#include <GL/glut.h>

#include "world.h"
#include "plane.h"

class world world(10000);
double view_factor = 1.0 / 1600.0;

void init() {
    for (int i = 0; i != world.size(); i++) {
        world[i].position.clear();
        world[i].velocity.clear();
        world[i].acceleration.clear();
        //world[i].position.x = (i%10-5) * 1.0;
        //world[i].position.y = i/100*1.0 + 100;
        //world[i].position.z = i%100/10 * 1.0;

        //world[i].position.x = (i%30-15) * 10;
        //world[i].position.y = i/30*10 + 300;

        world[i].position.y = i + 1400;
        world[i].position.x = 0;

        //world[i].position.y = i+2000;
        //world[i].position.x = i+200;
    }
    world.add_npc(gravity)
        //.add_npc(interact)
        //.add_npc(plane{{0, 1, 0}, 400})
        //.add_npc(plane{{0, 0, 1}, -100})
        //.add_npc(plane{{0, 0, 1}, 100})
        //.add_npc(plane{{1, 1, 0}, 0})
        //.add_npc(plane{{1, -1, 0}, 0});

        .add_npc(plane{{0, 1, 0}, 0})
        .add_npc(plane{{1, 0, 0}, -100})
        .add_npc(plane{{-1, 0, 0}, -100})
        .add_npc(plane{{0, 0, 1}, -100})
        .add_npc(plane{{0, 0, -1}, -100});
}

void render() {
    world.step();

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f, 0.0f, 0.7f);
    glBegin(GL_POINTS);
    for (auto& i : world)
        glVertex3d(i.position.x * view_factor, i.position.y * view_factor, i.position.z * view_factor);
    glEnd();

    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
    glVertex3d(-1, 0, 0);
    glVertex3d(1, 0, 0);
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
