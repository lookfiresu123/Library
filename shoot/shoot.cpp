#include "shoot.h"

// 视窗类
bool Viewport::grabbed = false;
int Viewport::width = 0;
int Viewport::height = 0;
Camera Viewport::camera;
Time Viewport::time;

// 鼠标类
int Mouse::currentX = 0;
int Mouse::currentY = 0;
int Mouse::lastX = 0;
int Mouse::lastY = 0;
int Mouse::deltaX = 0;
int Mouse::deltaY = 0;
float Mouse::sensitivity = 0;
bool Mouse::leftButton = false;
bool Mouse::middleButton = false;
bool Mouse::rightButton = false;

// 键盘类
bool KeyBoard::key[256];

// 绘制一个立方体
void addCube() {
    //  第一个面
    glBegin(GL_QUADS);
    glColor3f(1, 0, 0);
    glVertex3f(1, 0, -5);
    glVertex3f(1, 1, -5);
    glVertex3f(0, 1, -5);
    glVertex3f(0, 0, -5);
    glEnd();
    //  第二个面
    glBegin(GL_QUADS);
    glColor3f(0, 1, 0);
    glVertex3f(1, 0, -5);
    glVertex3f(1, 1, -5);
    glVertex3f(1, 1, -6);
    glVertex3f(1, 0, -6);
    glEnd();
    // 第三个面
    glBegin(GL_QUADS);
    glColor3f(0, 0, 1);
    glVertex3f(1, 0, -6);
    glVertex3f(1, 1, -6);
    glVertex3f(0, 1, -6);
    glVertex3f(0, 0, -6);
    glEnd();
    // 第四个面
    glBegin(GL_QUADS);
    glColor3f(1, 0, 1);
    glVertex3f(0, 1, -6);
    glVertex3f(0, 0, -6);
    glVertex3f(0, 0, -5);
    glVertex3f(0, 1, -5);
    glEnd();
    // 第五个面
    glBegin(GL_QUADS);
    glColor3f(1, 1, 0);
    glVertex3f(0, 1, -6);
    glVertex3f(1, 1, -6);
    glVertex3f(1, 1, -5);
    glVertex3f(0, 1, -5);
    glEnd();
}

// 视窗类
void Viewport::init() {
    // 设置GLUT的回调函数
    glutReshapeFunc(Viewport::reshape);
    glutDisplayFunc(Viewport::display);
    glutIdleFunc(Viewport::display);

    // 注册键盘事件的相关回调，使得视窗类可以处理键盘事件
    glutKeyboardFunc(KeyBoard::keyDown);
    glutKeyboardUpFunc(KeyBoard::keyUp);

    // 注册鼠标事件的相关回调，使得视窗类可以处理鼠标事件
    glutMotionFunc(Mouse::move);
    glutPassiveMotionFunc(Mouse::move); // 梳理鼠标的移动事件
    glutMouseFunc(Mouse::click);        // 处理鼠标的点击事件

    glEnable(GL_DEPTH_TEST);        // 启用深度测试，根据坐标的远近自动隐藏被遮住的图形
    glEnable(GL_TEXTURE_2D);        // 启用二维文理
    grabbed ? Viewport::setGrabbed(true) : Viewport::setGrabbed(false); // 配置鼠标是否直接被OPENGL全局捕获并显示

    camera.position.y = 1.75;
    Resources::load();
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
    processEvents();            // 处理鼠标、键盘事件等相关回调

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

    addCube();	// 绘制一个立方体

    // Resources::load();
    glBindTexture(GL_TEXTURE_2D, Resources::tex->textureID);    // 绑定材质
    glBegin(GL_QUADS);
    glColor3f(1, 1, 1);

    glTexCoord2f(100, 100);
    glVertex3f(100, 0, 100);

    glTexCoord2f(100, -100);
    glVertex3f(-100, 0, 100);

    glTexCoord2f(-100, -100);
    glVertex3f(-100, 0, -100);

    glTexCoord2f(-100, 100);
    glVertex3f(100, 0, -100);

    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);    // 结束绑定
    glutSwapBuffers();                  // 双缓冲显示模式
}

void Viewport::processEvents() {
    Mouse::update();                    // 根据鼠标更新渲染视窗
    // 根据鼠标来处理相机的移动
    // 相机的轴旋转角度 = 原角度 + 鼠标移动距离 * 鼠标灵敏度
    camera.rotation.x += static_cast<float>(Mouse::deltaX * Mouse::sensitivity);
    camera.rotation.y += static_cast<float>(Mouse::deltaY * Mouse::sensitivity);

    float WALKING_SPEED = 2.5;          // 移动速度

    // 设置时间标尺
    time.current = static_cast<float>(glutGet(GLUT_ELAPSED_TIME)) / 1000;
    time.delta = time.current - time.last;
    time.last = time.current;

    if (KeyBoard::key[static_cast<int>('w')]) {
        camera.position.x += (WALKING_SPEED * time.delta) * Math::dsin(camera.rotation.y);
        camera.position.z += (WALKING_SPEED * time.delta) * Math::dcos(camera.rotation.y);
    }
    if (KeyBoard::key[static_cast<int>('s')]) {
        camera.position.x += (WALKING_SPEED * time.delta) * Math::dsin(camera.rotation.y + 180);
        camera.position.z += (WALKING_SPEED * time.delta) * Math::dcos(camera.rotation.y + 180);
    }
    if (KeyBoard::key[static_cast<int>('a')]) {
        camera.position.x += (WALKING_SPEED * time.delta) * Math::dsin(camera.rotation.y + 270);
        camera.position.z += (WALKING_SPEED * time.delta) * Math::dcos(camera.rotation.y + 270);
    }
    if (KeyBoard::key[static_cast<int>('d')]) {
        camera.position.x += (WALKING_SPEED * time.delta) * Math::dsin(camera.rotation.y + 90);
        camera.position.z += (WALKING_SPEED * time.delta) * Math::dcos(camera.rotation.y + 90);
    }
}

// 鼠标类
void Mouse::click(int button, int state, int x, int y) {
    // 当鼠标点击左键时，响应这部分逻辑
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            leftButton = true;          // 设置鼠标左键被按下
            // 仅当点击位置位于GLUT窗口内才处理此事件
            if (x >= 0 && Viewport::width && y >= 0 && y < Viewport::height)
                Viewport::setGrabbed(true);
        }
        // 当鼠标按键被抬起时，将左键设置为false
        if (state == GLUT_UP)   leftButton = false;
    }
    // 当鼠标中间按键被按下时，响应这部分逻辑
    if (button == GLUT_MIDDLE_BUTTON) {
        if (state == GLUT_DOWN) middleButton = true;
        if (state == GLUT_UP)   middleButton = false;
    }
    // 当鼠标右键被按下时，响应这部分逻辑
    if (button == GLUT_RIGHT_BUTTON) {
        if (button == GLUT_DOWN)rightButton = true;
        if (button == GLUT_UP)  rightButton = false;
    }
}

void Mouse::move(int x, int y) {
    currentX = x;
    currentY = y;
}

void Mouse::update() {
    // 视角变换仅在鼠标被GLUT捕获时才激活
    if (Viewport::grabbed) {
        // 计算鼠标前后的变化
        deltaX = currentX - lastX;
        deltaY = currentY - lastY;

        // 更新本次记录
        lastX = currentX;
        lastY = currentY;

        // 处理当鼠标移出窗口的情况，对位置进行置位
        if (currentX <= 0 || currentX >= Viewport::width) {
            lastX -= currentX - Viewport::width/2;
            glutWarpPointer(Viewport::width/2, currentY);
        }
        if (currentY <= 0 || currentY >= Viewport::height) {
            lastY -= currentY - Viewport::height/2;
            glutWarpPointer(currentX, Viewport::height/2);
        }
    }
}

// 键盘类
void KeyBoard::keyDown(unsigned char k, int x, int y) {
    key[k] = true;
}

void KeyBoard::keyUp(unsigned char k, int x, int y) {
    key[k] = false;
}

// 材质类
Texture::Texture(void *data, int w, int h, int format) {
    glGenTextures(1, &textureID);               // 用来生成纹理
    glBindTexture(GL_TEXTURE_2D, textureID);    // 绑定材质
    // 生成一个2D纹理
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, format, GL_UNSIGNED_BYTE, data);

    // 过滤纹理
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);    // 结束绑定
    // 将数据、宽高、格式初始化到Texture类
    this->data = data;
    this->w = w;
    this->h = h;
    this->format = format;
}

Texture *Texture::loadBMP(const char *filename) {
    FILE *fp;       // 定义文件指针
    fp = fopen(filename, "r");  // 读取文件
    // 如果读取失败，则返回空指针，并关闭文件读取
    if (!fp) {
        cout << filename << " could not be opened!"<< endl;
        fclose(fp);
        return nullptr;
    }

    char headerField[3];
    fread(headerField, 2, sizeof(char), fp);    // 读取两个字节的内容
    if (strcmp(headerField, "BM")) {
        cout << "File is not a bitmap" << endl;
        fclose(fp);
        return nullptr;
    }

    unsigned int bmpDataLocation;   // 图像数据的位置
    unsigned int bmpWidth;          // 图像的宽
    unsigned int bmpHeight;         // 图像的高
    unsigned short numColorPlanes;  // 图像的颜色版
    unsigned short bitsPerPixel;    // 图像的像素
    unsigned int compressionMethod; // 图像的压缩方法
    unsigned int bmpDataSize;       // 图像的数据大小

    // 从0x000a开始，读取一个字节来获取图像的数据位置
    fseek(fp, 0x000a, SEEK_SET);
    fread(&bmpDataLocation, 1, sizeof(unsigned int), fp);

    // 从0x0012开始，依次读取一个字节来获取文件的基本信息
    fseek(fp, 0x0012, SEEK_SET);
    fread(&bmpWidth, 1, sizeof(unsigned int), fp);
    fread(&bmpHeight, 1, sizeof(unsigned int), fp);
    fread(&numColorPlanes, 1, sizeof(unsigned short), fp);
    fread(&bitsPerPixel, 1, sizeof(unsigned short), fp);
    fread(&compressionMethod, 1, sizeof(unsigned int), fp);
    fread(&bmpDataSize, 1, sizeof(unsigned int), fp);

    // 处理不符合格式的图像，并关闭退出
    if (numColorPlanes != 1 || bitsPerPixel != 24 || compressionMethod != 0) {
        cout << "File is not raw BMP24" << endl;
        fclose(fp);
        return nullptr;
    }

    unsigned char *bmpData = new unsigned char[bmpDataSize];    // 准备图像数据指针
    // 从图像数据位置处开始读取图像数据
    fseek(fp, bmpDataLocation, SEEK_SET);
    fread(bmpData, bmpDataSize, sizeof(unsigned char), fp);
    fclose(fp);

    // 读取完成后，创建一个Texture对象并返回
    return new Texture(bmpData, bmpWidth, bmpHeight, GL_BGR);
}

// 资源类
Texture *Resources::tex = nullptr;

void Resources::load() {
    tex = Texture::loadBMP("map.bmp");
}
