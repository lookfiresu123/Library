#include "solar_system.h"

SolarSystem solarsystem;

// 用于注册GLUT的回调
void onDisplay(void) {
    solarsystem.onDisplay();
}

void onUpdate(void) {
    solarsystem.onUpdate();
}

void onKeyboard(unsigned char key, int x, int y) {
    solarsystem.onKeyboard(key, x, y);
}

int main(int argc, char *argv[]) {
    // 对GLUT进行初始化，并处理所有命令行参数
    glutInit(&argc, argv);
    // 指定使用RGBA模式颜色索引，并指定使用双缓冲窗口
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    // 设置窗口被创建时左上角位于屏幕上的位置
    glutInitWindowPosition(WINDOW_X_POS, WINDOW_Y_POS);
    // 设置窗口被创建时的高度
    glutInitWindowSize(WIDTH, HEIGHT);
    // 创建一个窗口，输入的字符串为窗口的标题
    glutCreateWindow("SolarSystem at Nanjing University");

    // 每当GLUT确定一个窗口的内容需要更新显示时，glutDisplayFunc
    // 注册的回调函数onDisplay就会被执行
    glutDisplayFunc(onDisplay);
    // glutIdleFunc将指定一个函数，当当前事件循环处于空闲的时候
    // 就执行改函数，该回调函数接受一个函数指针作为它的唯一参数
    glutIdleFunc(onUpdate);
    // glutKeyboardFunc将键盘的键与一个函数关联，当这个键被按下
    // 或释放时，函数就会调用
    glutKeyboardFunc(onKeyboard);

    glutMainLoop();
    return 0;
}

