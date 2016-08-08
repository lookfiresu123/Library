#ifndef _H_SHOOT
#define _H_SHOOT 1

#include <iostream>
#include <GL/glut.h>
#include <cmath>
#include <cstring>

using std::cin;
using std::cout;
using std::endl;

#define PI 3.1415926

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

// 时间结构
typedef struct Time {
    float last;
    float current;
    float delta;
    Time() : last(0), current(0), delta(0) {}
} TIme;

class Math {
public:
    static double degressToRadians(double degress) {
        return degress * PI / 180;
    }
    static double dsin(double theta) {
        return sin(degressToRadians(theta));
    }
    static double dcos(double theta) {
        return cos(degressToRadians(theta));
    }
};

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
    static void processEvents();        // 处理事件
private:
    static Camera camera;               // 相机的基本属性
    static Time time;                   // 时间标尺
};


// 鼠标类，负责响应鼠标活动
class Mouse {
public:
    // 纪录当前鼠标位置
    static int currentX;
    static int currentY;

    // 纪录鼠标的上一次位置
    static int lastX;
    static int lastY;

    // 纪录鼠标移动的变化量
    static int deltaX;
    static int deltaY;

    // 纪录鼠标移动的灵敏度
    static float sensitivity;

    // 鼠标的三个按键
    static bool leftButton;
    static bool middleButton;
    static bool rightButton;

    static void move(int x, int y);                         // 鼠标移动方法
    static void update();                                   // 更新鼠标的相关事件信息
    static void click(int button, int state, int x, int y); // 鼠标点击方法
};


// 键盘类，负责响应键盘活动
class KeyBoard {
public:
    static bool key[256];
    static void keyDown(unsigned char k, int x, int y);     // 按键
    static void keyUp(unsigned char k, int x, int y);       // 抬键
};

// 材质类
class Texture {
public:
    unsigned int textureID; // 给出加载材质的ID
    void *data;             // 存储材质数据
    int w;                  // 材质的宽度
    int h;                  // 材质的高度
    unsigned char format;   // 材质的格式
    Texture(void *data, int w, int h, int format);  // 材质的构造方法
    static Texture *loadBMP(const char *filename);  // 从BMP图形中加载材质
};

// 资源类，在Viewport加载前加载
class Resources {
public:
    static Texture *tex;    // 材质指针
    static void load();     // 加载材质
    static void regen();    // 重新加载
};

#endif
