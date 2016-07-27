#include "shoot.h"

const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 500;
const char *WINDOW_TITLE = "CS Shooting Game";

int main(int argc, char *argv[]) {
    // 初始化 GLUT 和 OpenGL
    glutInit(&argc, argv);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow(WINDOW_TITLE); // 创建OpenGL
    Viewport::init();               // 初始化视窗
    glutMainLoop();                 // 主循环

    return 0;
}
