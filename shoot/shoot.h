#ifndef _H_SHOOT
#define _H_SHOOT 1

#include <iostream>
#include <GL/glut.h>

using std::cin;
using std::cout;
using std::endl;

// 三维空间中的点
typedef struct Vector3f {
    float x = 0;
    float y = 0;
    float z = 0;
} Vector3f;

// 摄像机类
typedef struct Camera {
    float maxTilt;              // 摄像机的最大倾斜角
    Vector3f position;          // 摄像机的位置
    Vector3f rotation;          // 摄像机的旋转
    Camera() : maxTilt(85) {}   // 构造函数
} Camera;

// 视窗类，负责刷新整个视图
class Viewport {
public:
    static bool grabbed;                // 鼠标指针是否显示
    static int width;                   // 视角宽度
    static int height;                  // 视角高度
    static void init();                 // 基本的初始化
    static void reshape(int w, int h);  // 配置窗口可调整
    static void display();              // 负责显示
    static void setGrabbed(bool value); // 设置鼠标指针是否显示
private:
    static Camera camera;               // 相机的基本属性
};


// 鼠标类，负责响应鼠标活动
// 键盘类，负责响应键盘活动

#endif
