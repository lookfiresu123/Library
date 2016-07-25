#ifndef _H_SOLAR_SYSTEM
#define _H_SOLAR_SYSTEM 1

// 使用GLUT的基本头文件
#include <GL/glut.h>
#include <cmath>

// 创建图形窗口的基本宏
#define WINDOW_X_POS 50
#define WINDOW_Y_POS 50
#define WIDTH 700
#define HEIGHT 700

#define PI 3.1415926535

// 观察者初始视角
#define REST 700
#define REST_Y (-REST)
#define REST_Z (REST)

// 自转速度
#define TIMEPAST 1      // 假设每次更新都经过一天
#define SELFOTATE 3

// 太阳系中的星球数量
#define STAR_NUM 10

// 星球名
enum STARS {
    Sun,        // 太阳
    Mercury,    // 水星
    Venus,      // 金星
    Earth,      // 地球
    Moon,       // 月球
    Mars,       // 火星
    Jupiter,    // 木星
    Saturn,     // 土星
    Uranus,     // 天王星
    Neptune     // 海王星
};

// 各星球的公转半径
#define SUN_RADIUS 48.74
#define MER_RADIUS 7.32
#define VEN_RADIUS 18.15
#define EAR_RADIUS 19.13
#define MOO_RADIUS 6.15
#define MAR_RADIUS 10.19
#define JUP_RADIUS 42.90
#define SAT_RADIUS 36.16
#define URA_RADIUS 25.56
#define NEP_RADIUS 24.78

// 距太阳的距离
#define MER_DIS 62.06
#define VEN_DIS 115.56
#define EAR_DIS 168.00
#define MOO_DIS 26.01
#define MAR_DIS 228.00
#define JUP_DIS 333.40
#define SAT_DIS 428.10
#define URA_DIS 848.00
#define NEP_DIS 949.10

// 移动速度
#define MER_SPEED 87.0
#define VEN_SPEED 225.0
#define EAR_SPEED 365.0
#define MOO_SPEED 30.0
#define MAR_SPEED 687.0
#define JUP_SPEED 1298.4
#define SAT_SPEED 3225.6
#define URA_SPEED 3066.4
#define NEP_SPEED 6014.8

// 自转速度
#define SELFROTATE 3

#define OFFSET 20

// 定义设置数组的宏
#define SET_VALUE_3(name, value0, value1, value2) \
    ((name)[0])=(value0), ((name)[1])=(value1), ((name)[2])=(value2)

// 星球类
class Star {
public:
    GLfloat radius;         // 星球的运转半径
    GLfloat distance;       // 星球中心与父结点星球中心的距离
    GLfloat speed;          // 公转速度（按角度来算）
    GLfloat selfspeed;      // 自转速度（按角度来算）
    Star *parentstar;       // 父结点星球
    GLfloat rgbacolor[4];   // 星球颜色

    // 构造函数，用于构造一颗星球
    Star(GLfloat radius_, GLfloat distance_,
         GLfloat speed_, GLfloat selfspeed_,
         Star *parent_);
    virtual ~Star() {}

    // 对一颗星球的移动和旋转进行绘制
    void drawStar();

    // 提供默认实现，赋值调用drawStar()
    virtual void draw() { drawStar(); }
    virtual void update(long timeSpan);
protected:
    GLfloat alphaself, alpha;
};

// 行星类
class Planet : public Star {
public:
    // 构造函数
    Planet(GLfloat radius_, GLfloat distance_,
           GLfloat speed_, GLfloat selfspeed_,
           Star *parent_, GLfloat rgbcolor_[3]);
    virtual ~Planet() {}

    // 增加对自身材质行星的绘制材料
    void drawPlanet();
    // 向自雷开方重写功能
    virtual void draw() { drawPlanet(); drawStar(); }
};

// 恒星类
class LightPlanet : public Planet {
public:
    LightPlanet(GLfloat radius_, GLfloat distance_,
                GLfloat speed_, GLfloat selfspeed_,
                Star *parent_, GLfloat rgbcolor_[3]);
    ~LightPlanet() {}

    // 增加对恒心的绘制光照
    void drawLight();
    virtual void draw() { drawLight(); drawPlanet(); drawStar(); }

};

// 太阳系类
class SolarSystem {
public:
    SolarSystem();
    ~SolarSystem();

    void onDisplay();
    void onUpdate();
    void onKeyboard(unsigned char key, int x, int y);
private:
    Star *stars[STAR_NUM];
    GLdouble viewX, viewY, viewZ;       // 观察者视角位置
    GLdouble centerX, centerY, centerZ; // 太阳位置
    GLdouble upX, upY, upZ;             // 观察者顶部朝上的方向（可以歪着看）
};

#endif
