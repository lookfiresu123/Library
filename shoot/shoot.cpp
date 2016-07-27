#include "shoot.h"

// 视窗类
bool Viewport::grabbed = false;
int Viewport::width = 0;
int Viewport::height = 0;

Camera Viewport::camera;

void Viewport::init() {
    // 设置GLUT的回调函数
    glutReshapeFunc(Viewport::reshape);
    glutDisplayFunc(Viewport::display);
    glutIdleFunc(Viewport::display);

    // 启用深度测试，根据坐标的远近自动隐藏被遮住的图形
    glEnable(GL_DEPTH_TEST);
    // 启用二维文理
    glEnable(GL_TEXTURE_2D);
    // 配置鼠标是否直接被OPENGL全局捕获并显示
    grabbed ? Viewport::setGrabbed(true) : Viewport::setGrabbed(false);
}

void Viewport::setGrabbed(bool value) {
    grabbed = value ? true : false;
    if (grabbed)    glutSetCursor(GLUT_CURSOR_NONE);        // 禁用光标
    else            glutSetCursor(GLUT_CURSOR_INHERIT);     // 启用光标
}

void Viewport::reshape(int w, int h) {
    width = w, height = h;
    if (h <= 0)    h = 1;
    float ratio = static_cast<float>(w) / h;   // 计算长宽比

    glMatrixMode(GL_PROJECTION);                // 切换矩阵模式
    glLoadIdentity();
    glViewport(0, 0, w, h);                     // 将PROJECTION加载为单位矩阵
    gluPerspective(45, ratio, 0.01, 1000);      // 调整透视角
    glMatrixMode(GL_MODELVIEW);                 // 切回GL_MODELVIEW
}

void Viewport::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清空当前Viewport中的缓冲
    glLoadIdentity();           // 重置为单位矩阵

    // 初始化相机的基本属性
    glRotatef(camera.rotation.x, 1, 0, 0);
    glRotatef(camera.rotation.y, 0, 1, 0);
    glRotatef(camera.rotation.z, 0, 0, 1);

    // 设置平移矩阵变换
    glTranslatef(-camera.position.x, -camera.position.y, camera.position.z);

    // 绘制一个三角形
    glBegin(GL_TRIANGLES);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(-1, 0, -3);
        glColor3f(0.0, 1.0, -3);
        glVertex3f(0, 2, -3);
        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(1, 0, -3);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);    // 绑定材质
    glutSwapBuffers();                  // 双缓冲显示模式
}
