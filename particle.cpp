#include <array>
#include <cstdlib>
#include <ctime>
#include <random>
#include <GL/gl.h>
#include <GL/glut.h>

#include "vector3d.hpp"

#define sq(x) ((x)*(x))
#define qq(x) ((x)*(x)*(x)*(x))
#define hq(x) ((x)*(x)*(x)*(x)*(x)*(x))

constexpr double PI  = 3.1415927;
constexpr double Km  = 0.2;
constexpr double Kn  = 40;
constexpr double ETA = 0.3;
constexpr double MU  = 0.25 * PI;
constexpr double D   = 1;
constexpr double Dm  = 100 * D;
constexpr double Dw  = 50 * D;
constexpr double T   = 0.005;
constexpr vector3d G = {0, -10, 0};

double view_factor = 1.0 / 1600.0;

struct particle {
    vector3d position;
    vector3d velocity;
    vector3d acceleration;
};

struct plane {
    struct vector3d normal;
    double C;
};

std::array<particle, 1000> fluid;

std::mt19937 gen(std::random_device{}());

inline double frand() {
    static std::uniform_real_distribution<> dis(0, 1);
    return dis(gen);
}

inline int rs() {
    return frand()>0.5 ? 1 : -1;
}

void clear() {
    for (auto& i : fluid)
        i.acceleration.clear();
}

void interact() {
    for (auto& i : fluid) {
        for (auto& j : fluid) {
            if (&i != &j) {
                auto a = j.position;
                a -= i.position;
                auto d = a.length();
                if (d == 0) {
                    continue;
                }
                else if (d < D) {
                    a.set_length(-Km / d);
                }
                else if (d >= D && d < Dm) {
                    a.set_length(Km / hq(d));
                }
                else {
                    a.clear();
                }
                i.acceleration += a;
            }
        }
    }
}

void gravity() {
    for (auto& i : fluid)
        i.acceleration += G;
}

void container(const std::vector<plane> planes) {
    for (auto& i : fluid) {
        for (const auto& p : planes) {
            auto norm = p.normal;
            auto lam = (norm*i.position + p.C) / (norm*norm);
            norm *= lam;
            double d = norm.length();
            if (d < Dw) {
                auto r = std::tan(frand()*MU) * norm.length();
                auto ytoz = std::atan2(norm.y, norm.z);
                auto ztox = std::atan2(norm.z, norm.x) - 0.5*PI;
                vector3d dn;
                dn.x = rs() * frand() * r;
                dn.y = rs() * std::sqrt(sq(r)-sq(dn.x));
                dn.z = 0;
                dn.rotate_x(-ytoz);
                dn.rotate_y(ztox);
                norm += dn;
                norm.set_length(Kn * (Dw-d));
                if (norm*i.velocity / (norm.length()*i.velocity.length()) > 0)
                    norm *= ETA;
                i.acceleration += norm;
            }
        }
    }
}

void update() {
    for (auto& i : fluid) {
        vector3d s = i.velocity;
        s *= T;
        vector3d s2 = i.acceleration;
        s += (s2 *= (sq(T) * 0.5));
        vector3d dv = i.acceleration;
        dv *= T;
        i.position += s;
        i.velocity += dv;
    }
}

void step() {
    static std::vector<plane> planes = {
        {{0, 1, 0},  400},
        {{0, 0, 1},  -100},
        {{0, 0, 1},  100},
        {{1, 1, 0},  0},
        {{1, -1, 0},  0}
    };
    clear();
    interact();
    gravity();
    container(planes);
    update();
}

void init() {
    for (int i = 0; i != fluid.size(); i++) {
        fluid[i].position.clear();
        fluid[i].velocity.clear();
        fluid[i].acceleration.clear();
        //fluid[i].position.x = (i%10-5) * 1.0;
        //fluid[i].position.y = i/100*1.0 + 100;
        //fluid[i].position.z = i%100/10 * 1.0;

        //fluid[i].position.x = (i%30-15) * 1.2;
        //fluid[i].position.y = i/30*1.2 + 300;
        
        fluid[i].position.y = i+800;
        fluid[i].position.x = 400;
    }
}

void render() {
    step();

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0, 0.7);
    glBegin(GL_POINTS);
    for (auto& i : fluid)
        glVertex3d(i.position.x * view_factor, i.position.y * view_factor, i.position.z * view_factor);
    glEnd();

    glColor3f(1, 1, 1);
    //glBegin(GL_LINES);
    //glVertex3d(-1, -400 * view_factor, 0);
    //glVertex3d(1, -400 * view_factor, 0);
    //glEnd();

    //glBegin(GL_LINES);
    //glVertex3d(100 * view_factor, 1, 0);
    //glVertex3d(100 * view_factor, -1, 0);
    //glEnd();

    //glBegin(GL_LINES);
    //glVertex3d(-100 * view_factor, 1, 0);
    //glVertex3d(-100 * view_factor, -1, 0);
    //glEnd();

    glBegin(GL_LINES);
    glVertex3d(-1, 1, 0);
    glVertex3d(1, -1, 0);
    glEnd();

    glBegin(GL_LINES);
    glVertex3d(-1, -1, 0);
    glVertex3d(1, 1, 0);
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
