#include "solar_system.h"

// 星球类
Star::Star(GLfloat radius_, GLfloat distance_,
           GLfloat speed_, GLfloat selfspeed_,
           Star *parent_) :
    radius(radius_), distance(distance_), speed(360/speed_),
    selfspeed(selfspeed_), parentstar(parent_) {}

// 绘制星球
void Star::drawStar() {
    // 激活OPENGL中的各种功能
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    int n = 1440;
    glPushMatrix();

    // 公转
    // 若星球为行星，且距离不为0，则需向原地平移一个半径
    if (parentstar != 0 && parentstar->distance > 0) {
        // 这部分用于处理卫星对象
        // 将绘制的图形眼z轴旋转alpha
        glRotatef(parentstar->alpha, 0, 0, 1);
        // x轴方向上平移distance，且y,z方向不变
        glTranslatef(parentstar->distance, 0.0, 0.0);
    }
    // 绘制运行轨道
    glBegin(GL_LINES);
    for (int i = 0 ; i < n ; ++i)
        glVertex2f(distance*cos(2*PI*i/n), distance*sin(2*PI*i/n));
    glEnd();

    // 绕z轴旋转alpha
    glRotatef(alpha, 0, 0, 1);
    // x轴方向平移distance，且y,z方向不变
    glTranslatef(distance, 0.0, 0.0);

    // 自转
    glRotatef(alphaself, 0, 0, 1);
    // 绘制行星颜色
    glColor3f(rgbacolor[0], rgbacolor[1], rgbacolor[2]);
    glutSolidSphere(radius, 40, 32);

    glPopMatrix();
}

void Star::update(long timeSpan) {
    alpha += timeSpan * speed;  // 更新公转角度
    alphaself += selfspeed;     // 更新自转角度
}

// 行星类的构造函数
Planet::Planet(GLfloat radius_, GLfloat distance_,
               GLfloat speed_, GLfloat selfspeed_,
               Star *parent_, GLfloat rgbcolor_[3]) :
    Star(radius_, distance_, speed_, selfspeed_, parent_) {
    SET_VALUE_3(rgbacolor, rgbcolor_[0], rgbcolor_[1], rgbcolor_[2]);
}

// 给行星绘制光照效果
void Planet::drawPlanet() {
    GLfloat mat_ambient[] = {0.0f, 0.0f, 0.5f, 1.0f};
    GLfloat mat_diffuse[] = {0.0f, 0.0f, 0.5f, 1.0f};
    GLfloat mat_specular[] = {0.0f, 0.0f, 1.0f, 1.0f};
    GLfloat mat_emission[] = {rgbacolor[0], rgbacolor[1], rgbacolor[2], rgbacolor[3]};
    GLfloat mat_shininess = 90.0f;
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
}

// 恒星类的构造函数
LightPlanet::LightPlanet(GLfloat radius_, GLfloat distance_,
        GLfloat speed_, GLfloat selfspeed_, Star *parent_,
        GLfloat rgbcolor_[3]) :
    Planet(radius_, distance_, speed_, selfspeed_, parent_, rgbcolor_) {}

// 设置恒星的光照材料和光源位置
void LightPlanet::drawLight() {
    GLfloat light_position[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat light_ambient[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);  // 指定零号光源的位置
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);    // 表示各种光线照射到该材料上，经过多次反射后遗留的光线强度
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);    // 漫反射后的光照强度
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);  // 镜面反射后的光照强度
}

SolarSystem::SolarSystem() {
    viewX = 0, viewY = REST_Y, viewZ = REST_Z;
    centerX = centerY = centerZ = 0;
    upX = upY = 0, upZ = 1;

    // 太阳
    GLfloat rgbcolor_[3] = {1, 0, 0};
    stars[Sun] = new LightPlanet(SUN_RADIUS,0,0,SELFROTATE,0,rgbcolor_);

    // 水星
    SET_VALUE_3(rgbcolor_, .2, .2, .5);
    stars[Mercury] = new Planet(MER_RADIUS, MER_DIS, MER_SPEED, SELFROTATE, stars[Sun], rgbcolor_);

    // 金星
    SET_VALUE_3(rgbcolor_, 1, 0.7, 0);
    stars[Venus] = new Planet(VEN_RADIUS, VEN_DIS, VEN_SPEED, SELFROTATE, stars[Sun], rgbcolor_);

    // 地球
    SET_VALUE_3(rgbcolor_, 0, 1, 0);
    stars[Earth] = new Planet(EAR_RADIUS, EAR_DIS, EAR_SPEED, SELFROTATE, stars[Sun], rgbcolor_);

    // 月球
    SET_VALUE_3(rgbcolor_, 1, 1, 0);
    stars[Moon] = new Planet(MOO_RADIUS, MOO_DIS, MOO_SPEED, SELFROTATE, stars[Earth], rgbcolor_);

    // 火星
    SET_VALUE_3(rgbcolor_, 1, .5, .5);
    stars[Mars] = new Planet(MAR_RADIUS, MAR_DIS, MAR_SPEED, SELFROTATE, stars[Sun], rgbcolor_);

    // 木星
    SET_VALUE_3(rgbcolor_, 1, 1, .5);
    stars[Jupiter] = new Planet(JUP_RADIUS, JUP_DIS, JUP_SPEED, SELFROTATE, stars[Sun], rgbcolor_);

    // 土星
    SET_VALUE_3(rgbcolor_, .5, 1, .5);
    stars[Saturn] = new Planet(SAT_RADIUS, SAT_DIS, SAT_SPEED, SELFROTATE, stars[Sun], rgbcolor_);

    // 天王星
    SET_VALUE_3(rgbcolor_, .4, .4, .4);
    stars[Uranus] = new Planet(URA_RADIUS, URA_DIS, URA_SPEED, SELFROTATE, stars[Sun], rgbcolor_);

    // 海王星
    SET_VALUE_3(rgbcolor_, .5, .5, 1);
    stars[Neptune] = new Planet(NEP_RADIUS, NEP_DIS, NEP_SPEED, SELFROTATE, stars[Sun], rgbcolor_);
}

SolarSystem::~SolarSystem() {
    for (int i = 0 ; i < STAR_NUM ; ++i)
        delete stars[i];
}

void SolarSystem::onDisplay() {
    // 清除viewport缓冲区
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 清除并设置颜色缓存
    glClearColor(.7f, .7f, .7f, .1f);
    // 指点当前矩阵为投影矩阵
    glMatrixMode(GL_PROJECTION);
    // 将指定的矩阵指定为单位矩阵
    glLoadIdentity();
    // 指定当前的观察视景体
    gluPerspective(75.0f, 1.0f, 1.0f, 40000000);
    // 指定当前矩阵为视景矩阵堆栈应用术后的矩阵操作
    glMatrixMode(GL_MODELVIEW);
    // 指定当前的矩阵为单位矩阵
    glLoadIdentity();
    // 定义视图矩阵，并与当前矩阵相乘
    gluLookAt(viewX, viewY, viewZ, centerX, centerY, centerZ, upX, upY, upZ);

    // 设置第一个光源（0号光源）
    glEnable(GL_LIGHT0);
    // 启用光源
    glEnable(GL_LIGHTING);
    // 启动深度测试，根据坐标的远近自动隐藏被遮住的图形
    glEnable(GL_DEPTH_TEST);

    // 绘制星球
    for (int i = 0 ; i < STAR_NUM ; ++i)
        stars[i]->draw();

    // 实现双缓冲的缓冲区交换
    glutSwapBuffers();
}

void SolarSystem::onUpdate() {
    for (int i = 0 ; i < STAR_NUM ; ++i)
        stars[i]->update(TIMEPAST);     // 更新星球的位置
    onDisplay();    // 刷新显示
}

void SolarSystem::onKeyboard(unsigned char key, int x, int y) {
    switch(key) {
        case 'W':   viewY += OFFSET; break;
        case 's':   viewZ += OFFSET; break;
        case 'S':   viewZ -= OFFSET; break;
        case 'a':   viewX -= OFFSET; break;
        case 'd':   viewX += OFFSET; break;
        case 'x':   viewY -= OFFSET; break;
        case 'r':
            viewX = 0, viewY = REST_Y, viewZ = REST_Z;
            centerX = centerY = centerZ = 0;
            upX = upY = 0, upZ = 1;
            break;
        case 27:    exit(0); break;
        default:    break;
    }
}
