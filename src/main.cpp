#include <stdlib.h>
#include <iostream>
// OSX systems need their own headers
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
// Use of degrees is deprecated. Use radians for GLM functions
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "Screenshot.h"
#include "Scene.h"
using namespace glm;
using namespace std;

static const int width = 800;
static const int height = 600;
static const char* title = "Scene viewer";
static const vec4 background(0.1f, 0.2f, 0.3f, 1.0f);
static Scene scene;

// New keys for gravititional effect
static bool animate;
static bool ellipsoid;

// constants for Buss's second-order method
static vec3 w;
static mat3 M_model;
static mat3 M_world;
static vec3 L;

#include "hw3AutoScreenshots.h"

void printHelp() {
    std::cout << R"(
    Available commands:
      press 'H' to print this message again.
      press Esc to quit.
      press 'O' to save a screenshot.
      press the arrow keys to rotate camera.
      press 'A'/'Z' to zoom.
      press 'R' to reset camera.
      press 'L' to turn on/off the lighting.
      press Spacebar to generate images for hw3 submission.
      press 'X' to start/stop the animation
      press 'E' to toggle ellipsoids on and off
    
)";
}

void initialize(void) {
    printHelp();
    glClearColor(background[0], background[1], background[2], background[3]); // background color
    glViewport(0, 0, width, height);

    animate = false;
    ellipsoid = true;

    // Initialize scene
    scene.init();
    //scene.shader->enablelighting = true;
    w = scene.w;
    M_model = scene.M_model;

    // compute initial values for Buss's algorithm constants
    mat3 R_init(scene.node["world"]->childtransforms[0]);
    M_world = R_init * M_model * transpose(R_init);
    L = M_world * w;

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
}

void recomputePoinsot() {
    float E_k = dot(w, (M_model * w));
    float M = dot(w, (transpose(M_model) * M_model) * w);

    scene.node["EPoin"]->modeltransforms[0] = scale(vec3(sqrt(E_k / M_model[0][0]), sqrt(E_k / M_model[1][1]), sqrt(E_k / M_model[2][2])));
    scene.node["MPoin"]->modeltransforms[0] = scale(vec3(sqrt(M) / M_model[0][0], sqrt(M) / M_model[1][1], sqrt(M) / M_model[2][2]));
    if (!ellipsoid) {
        scene.node["MPoin"]->modeltransforms[0] *= scale(vec3(0.001f));
        scene.node["EPoin"]->modeltransforms[0] *= scale(vec3(0.001f));
    }
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    scene.draw();

    glutSwapBuffers();
    glFlush();

}

void saveScreenShot(const char* filename = "test.png") {
    int currentwidth = glutGet(GLUT_WINDOW_WIDTH);
    int currentheight = glutGet(GLUT_WINDOW_HEIGHT);
    Screenshot imag = Screenshot(currentwidth, currentheight);
    imag.save(filename);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 27: // Escape to quit
        exit(0);
        break;
    case 'h': // print help
        printHelp();
        break;
    case 'o': // save screenshot
        saveScreenShot();
        break;
    case 'r':
        scene.camera->aspect_default = float(glutGet(GLUT_WINDOW_WIDTH)) / float(glutGet(GLUT_WINDOW_HEIGHT));
        scene.camera->reset();
        glutPostRedisplay();
        break;
    case 'a':
        scene.camera->zoom(0.9f);
        glutPostRedisplay();
        break;
    case 'z':
        scene.camera->zoom(1.1f);
        glutPostRedisplay();
        break;
    case 'l':
        scene.shader->enablelighting = !(scene.shader->enablelighting);
        glutPostRedisplay();
        break;
    case ' ':
        hw3AutoScreenshots();
        glutPostRedisplay();
        break;
    case 'x':
        animate = !animate;
        glutPostRedisplay();
        break;
    case 'e':
        ellipsoid = !ellipsoid;
        if (ellipsoid) {
            scene.node["MPoin"]->modeltransforms[0] *= scale(vec3(1000.0f));
            scene.node["EPoin"]->modeltransforms[0] *= scale(vec3(1000.0f));
            scene.node["polhode"]->modeltransforms[0] *= scale(vec3(1000.0f));
        }
        else {
            scene.node["MPoin"]->modeltransforms[0] *= scale(vec3(0.001f));
            scene.node["EPoin"]->modeltransforms[0] *= scale(vec3(0.001f));
            scene.node["polhode"]->modeltransforms[0] *= scale(vec3(0.001f));
        }
        glutPostRedisplay();
        break;
    default:
        glutPostRedisplay();
        break;
    }
}
void specialKey(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP: // up
        scene.camera->rotateUp(-10.0f);
        glutPostRedisplay();
        break;
    case GLUT_KEY_DOWN: // down
        scene.camera->rotateUp(10.0f);
        glutPostRedisplay();
        break;
    case GLUT_KEY_RIGHT: // right
        scene.camera->rotateRight(-10.0f);
        glutPostRedisplay();
        break;
    case GLUT_KEY_LEFT: // left
        scene.camera->rotateRight(10.0f);
        glutPostRedisplay();
        break;
    }
}

// Buss's method to better conserve angular momentum
mat4 buss(mat4 R, vec3 w, mat3 M_model, float dt) {
    w = inverse(M_world) * L;
    vec3 a = -1.0f * inverse(M_world) * cross(w, L);
    vec3 w_bar = w + (dt / 2) * a + dt * dt * cross(a, w) / 12.0f;
    R = rotate(dt * length(w_bar), normalize(w_bar)) * R;
    
    mat3 R_mat3(R);
    M_world = R_mat3 * M_model * transpose(R_mat3);

    // update angular velocity
    scene.node["world"]->childtransforms[1] = translate(inverse(R_mat3) * w);

    return R;
}

// animation function to run once a frame
void animation(void) {
    static int prev_time = glutGet(GLUT_ELAPSED_TIME);
    int curr_time = glutGet(GLUT_ELAPSED_TIME);

    float dt = ((float)curr_time - prev_time) / 1000.0f;
    prev_time = curr_time; // update previous time

    // pause animation if needed
    if (!animate) {
        return;
    }

    // Buss's algorithm
    mat4 R = scene.node["world"]->childtransforms[0];
    scene.node["world"]->childtransforms[0] = buss(R, w, M_model, dt);
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    // BEGIN CREATE WINDOW
    glutInit(&argc, argv);

#ifdef __APPLE__
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
#else
    //glutInitContextVersion(3, 1);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
#endif
    glutInitWindowSize(width, height);
    glutCreateWindow(title);
#ifndef __APPLE__
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
    }
#endif
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    // END CREATE WINDOW

    initialize();
    glutDisplayFunc(display);
    glutIdleFunc(animation);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKey);

    glutMainLoop();
    return 0;   /* ANSI C requires main to return int. */
}
