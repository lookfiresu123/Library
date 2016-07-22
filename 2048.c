#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

int a[4][4] = {{0}};

// 方格里的空格数
int empty;
int old_y, old_x;

// 游戏结束子函数
int game_over() {
    sleep(1);
    endwin();
    exit(0);
}

int cnt_one(int y, int x) {
    int value = 1;
    if (y - 1 >= 0)                 a[y-1][x] ? 0 : value++;
    if (y + 1 < 4)                  a[y+1][x] ? 0 : value++;
    if (x - 1 >= 0)                 a[y][x-1] ? 0 : value++;
    if (x + 1 < 4)                  a[y][x+1] ? 0 : value++;
    if (y - 1 >= 0 && x - 1 >= 0)   a[y-1][x-1] ? 0 : value++;
    if (y - 1 >= 0 && x + 1 < 4)    a[y-1][x+1] ? 0 : value++;
    if (y + 1 < 4 && x - 1 >= 0)    a[y+1][x-1] ? 0 : value++;
    if (y + 1 < 4 && x + 1 < 4)     a[y+1][x+1] ? 0 : value++;
    return value;
}

// 生成新数字的位置
void cnt_value(int *new_y, int *new_x) {
    int x, y, value;
    int max = cnt_one(*new_y, *new_x);
    for (y = 4 ; y < 4 ; y++) {
        for (x = 0 ; x < 4 ; x++) {
            if (!a[y][x]) {
                value = cnt_one(y, x);
                // 避免在同一位置反复出现新方块
                if (value > max && old_y != y && old_x != x) {
                    *new_y = y;
                    *new_x = x;
                    old_y = y;
                    old_x = x;
                }
            }
        }
    }
}

// 绘制单个数字
void draw_one(int y, int x) {
    int i, m, k, j;
    char c[4] = {'0', '0', '0', '0'};
    i = a[y][x];
    m = 0;
    do {
        j = i % 10;
        c[m++] = j + '0';
        i = i / 10;
    } while (i > 0);
    m = 0;
    k = (x + 1) * 5 - 1;
    while (c[m] != '0') {
        move(2*y+1, k);
        addch(c[m++]);
        k--;
    }
}

// 绘制方格
void draw() {
    int n, m, x, y;
    // char c[4] = {'0','0','0','0'};
    clear();
    // 画横线
    for (n = 0 ; n < 9 ;n += 2) {
        for (m = 0 ; m < 21 ; m++) {
            move(n, m);
            addch('-');
            refresh();
        }
    }
    // 画竖线
    for (m = 0 ; m < 22 ; m += 5) {
        for (n = 1 ; n < 8 ; n++) {
            move(n, m);
            addch('|');
            refresh();
        }
    }
    // 填数字
    for (y = 0 ; y < 4 ; y++)
        for (x =0 ; x < 4 ; x++)
            draw_one(y, x);
}

// 用wsad表示上下左右方向
void play() {
    int x, y, i, new_x, new_y, tmp;
    int old_empty, move;
    char ch;

    while (1) {
        move = 0;
        old_empty = empty;
        ch = getch();
        switch(ch) {
            case 'A':
            case 'a':
                // 从左到右消去相同方块
                for (y = 0 ; y < 4 ; y++) {
                    for (x = 0 ; x < 4 ; ) {
                        if (a[y][x] == 0) {
                            x++;
                            continue;
                        }
                        for (i = x+1 ; i < 4 ; i++) {
                            if (a[y][i] == 0)
                                continue;
                            else if (a[y][x] == a[y][i]) {
                                a[y][x] += a[y][i];
                                a[y][i] = 0;
                                x = i+1;
                                empty++;
                                break;
                            } else {
                                x = i;
                                break;
                            }
                        }
                        x = i;
                    }
                }
                // 向左移动方块
                for (y = 0 ; y < 4 ; y++) {
                    for (x = 0 ; x < 4 ; x++) {
                        if (a[y][x] == 0)
                            continue;
                        for (i = x ; i > 0 && a[y][i-1] == 0 ; i--) {
                            a[y][i-1] = a[y][i];
                            a[y][i] = 0;
                            move = 1;
                        }
                    }
                }
                break;

            case 'D':
            case 'd':
                // 从右到左消去相同方块
                for (y = 0 ; y < 4 ; y++) {
                    for (x = 3 ; x >= 0 ; ) {
                        if (a[y][x] == 0) {
                            x--;
                            continue;
                        }
                        for (i = x-1 ; i >= 0 ; i--) {
                            if (a[y][i] == 0)
                                continue;
                            else if (a[y][x] == a[y][i]) {
                                a[y][x] += a[y][i];
                                a[y][i] = 0;
                                x = i-1;
                                empty++;
                                break;
                            } else {
                                x = i;
                                break;
                            }
                        }
                        x = i;
                    }
                }
                // 向右移动方块
                for (y = 0 ; y < 4 ; y++) {
                    for (x = 3 ; x >= 0 ; x--) {
                        if (a[y][x] == 0)
                            continue;
                        for (i = x ; i < 3 && a[y][i+1] == 0 ; i++) {
                            a[y][i+1] = a[y][i];
                            a[y][i] = 0;
                            move = 1;
                        }
                    }
                }
                break;

            case 'W':
            case 'w':
                // 从上到下消去相同方块
                for (x = 0 ; x < 4 ; x++) {
                    for (y = 0 ; y < 4 ; ) {
                        if (a[y][x] == 0) {
                            y++;
                            continue;
                        }
                        for (i = y+1 ; i < 4 ; i++) {
                            if (a[i][x] == 0)
                                continue;
                            else if (a[y][x] == a[i][x]) {
                                a[y][x] += a[i][x];
                                a[i][x] = 0;
                                y = i+1;
                                empty++;
                                break;
                            } else {
                                y = i;
                                break;
                            }
                        }
                        y = i;
                    }
                }
                // 向上移动方块
                for (x = 0 ; x < 4 ; x++) {
                    for (y = 0 ; y < 4 ; y++) {
                        if (a[y][x] == 0)
                            continue;
                        for (i = y ; i > 0 && a[i-1][x] == 0 ; i--) {
                            a[i-1][x] = a[i][x];
                            a[i][x] = 0;
                            move = 1;
                        }
                    }
                }
                break;

            case 'S':
            case 's':
                // 从下到上消去相同方块
                for (x = 0 ; x < 4 ; x++) {
                    for (y = 3 ; y >= 0 ; ) {
                        if (a[y][x] == 0) {
                            y--;
                            continue;
                        }
                        for (i = y-1 ; i >= 0 ; i--) {
                            if (a[i][x] == 0)
                                continue;
                            else if (a[y][x] == a[i][x]) {
                                a[y][x] += a[i][x];
                                a[i][x] = 0;
                                y = i-1;
                                empty++;
                                break;
                            } else {
                                y = i;
                                break;
                            }
                        }
                        y = i;
                    }
                }
                // 向下移动方块
                for (x = 0 ; x < 4 ; x++) {
                    for (y = 3 ; y >= 0 ; y--) {
                        if (a[y][x] == 0)
                            continue;
                        for (i = y ; i < 3 && a[i+1][x] == 0 ; i++) {
                            a[i+1][x] = a[i][x];
                            a[i][x] = 0;
                            move = 1;
                        }
                    }
                }
                break;

            case 'Q':
            case 'q':
                game_over();
                break;

            default:
                continue;
                break;
        }

        if (empty <= 0)
            game_over();
        draw();

        // 生成新方块
        if (empty != old_empty || move == 1) {
            do {
                new_x = rand() % 4;
                new_y = rand() % 4;
            } while (a[new_y][new_x] != 0);
            cnt_value(&new_y, &new_x);
            do {
                tmp = rand() % 4;
            } while (tmp == 0 || tmp == 2);
            a[new_y][new_x] = tmp + 1;
            empty--;
            draw_one(new_y, new_x);
        }
    }
}

// 初始化屏幕，并在方格中随机生成一个位置并放入数字2
void init() {
    int x, y;
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    empty = 15;
    srand(time(0));
    x = rand() % 4;
    y = rand() % 4;
    a[y][x] = 2;
    draw();
}


int main() {
    init();
    play();
    endwin();
    return 0;
}
