#include "2048.h"

// 游戏类的构造函数
Game::Game(int x) : n(x), empty(n*n), old_pos(0,0), vec(n, vector<int>(n,0)) {}

// 游戏结束子函数
int Game::game_over() {
    sleep(1);
    endwin();
    exit(0);
}

int Game::cnt_one(pair<int, int> &pos) {
    int value = 1;
    if (pos.first-1 >= 0)                       vec[pos.first-1][pos.second] ? 0 : value++;
    if (pos.first+1 < n)                        vec[pos.first+1][pos.second] ? 0 : value++;
    if (pos.second-1 >= 0)                      vec[pos.first][pos.second-1] ? 0 : value++;
    if (pos.second+1 < n)                       vec[pos.first][pos.second+1] ? 0 : value++;
    if (pos.first-1 >= 0 && pos.second-1 >= 0)  vec[pos.first-1][pos.second-1] ? 0 : value++;
    if (pos.first-1 >= 0 && pos.second+1 < n)   vec[pos.first-1][pos.second+1] ? 0 : value++;
    if (pos.first+1 < n && pos.second-1 >= 0)   vec[pos.first+1][pos.second-1] ? 0 : value++;
    if (pos.first+1 < n && pos.second+1 < n)    vec[pos.first+1][pos.second+1] ? 0 : value++;
    return value;
}

// 生成新数字的位置
void Game::cnt_value(pair<int, int> &new_pos) {
    int value;
    int max = cnt_one(new_pos);
    for (int i = 0 ; i < n ; ++i) {
        for (int j = 0 ; j < n ; ++j) {
            if (!vec[i][j]) {
                pair<int, int> tmp(i, j);
                value = cnt_one(tmp);
                // 避免在同一位置反复出现新方块
                if (value > max && old_pos.first != i && old_pos.second != j)
                    new_pos = tmp, old_pos = tmp;
            }
        }
    }
}

// 绘制单个数字
void Game::draw_one(pair<int, int> &pos) {
    vector<char> chs(n, '0');
    int value = vec[pos.first][pos.second];
    int m = 0;
    do {
        chs[m++] = value % 10 + '0';
        value /= 10;
    } while (value > 0);
    m = 0;
    int k = (pos.second+1)*5 - 1;   // 疑问：用n+1代替5 ?
    while (chs[m] != '0') {
        move(2*pos.first+1, k);
        addch(chs[m++]);
        --k;
    }
}

// 绘制方格
void Game::draw() {
    clear();
    // 画横线
    for (int i = 0 ; i < 2*n+1 ; i += 2) {
        for (int j = 0 ; j < 5*n+1; ++j) {
            move(i, j);
            addch('-');
            refresh();
        }
    }
    // 画竖线
    for (int j = 0 ; j < 5*n+1 ; j += 5) {
        for (int i = 1 ; i < 2*n ; ++i) {
            move(i, j);
            addch('|');
            refresh();
        }
    }
    // 填数字
    pair<int, int> pos;
    for (int i = 0 ; i < n ; ++i)
        for (int j = 0 ; j < n ; ++j)
            pos.first = i, pos.second = j, draw_one(pos);
}

void Game::play() {
    pair<int, int> new_pos, pos;
    int i, j, k, tmp, old_empty, move;
    char ch;
    while (true) {
        move = 0;
        old_empty = empty;
        ch = getch();
        switch(ch) {
            case 'A':
            case 'a':
                // 从左到右消去相同方块
                for (i = 0 ; i < n ; ++i) {
                    for (j = 0 ; j < n ; ) {
                        if (vec[i][j] == 0) {
                            ++j;
                            continue;
                        }
                        for (k = j+1 ; k < n ; ++k) {
                            if (vec[i][k] == 0) continue;
                            else if (vec[i][j] == vec[i][k]) {
                                vec[i][j] += vec[i][k], vec[i][k] = 0;
                                j = k+1, ++empty;
                                break;
                            } else {
                                j = k;
                                break;
                            }
                        }
                        j = k;
                    }
                }
                // 向左移动方块
                for (i = 0 ; i < n ; ++i) {
                    for (j = 0 ; j < n ; ++j) {
                        if (vec[i][j] == 0)
                            continue;
                        for (k = j ; k > 0 && vec[i][k-1] == 0 ; --k) {
                            vec[i][k-1] = vec[i][k];
                            vec[i][k] = 0;
                            move = 1;
                        }
                    }
                }
                break;

            case 'D':
            case 'd':
                // 从右到左消去相同方块
                for (i = 0 ; i < n ; ++i) {
                    for (j = n-1 ; j >= 0 ; ) {
                        if (vec[i][j] == 0) {
                            --j;
                            continue;
                        }
                        for (k = j-1 ; k >= 0 ; k--) {
                            if (vec[i][k] == 0)
                                continue;
                            else if (vec[i][j] == vec[i][k]) {
                                vec[i][j] += vec[i][k];
                                vec[i][k] = 0;
                                j = k-1;
                                empty++;
                                break;
                            } else {
                                j = k;
                                break;
                            }
                        }
                        j = k;
                    }
                }
                // 向右移动方块
                for (i = 0 ; i < n ; ++i) {
                    for (j = n-1 ; j >= 0 ; --j) {
                        if (vec[i][j] == 0)
                            continue;
                        for (k = j ; k < n-1 && vec[i][k+1] == 0 ; ++k) {
                            vec[i][k+1] = vec[i][k];
                            vec[i][k] = 0;
                            move = 1;
                        }
                    }
                }
                break;

            case 'W':
            case 'w':
                // 从上到下消去相同方块
                for (j = 0 ; j < n ; ++j) {
                    for (i = 0 ; i < n ; ) {
                        if (vec[i][j] == 0) {
                            ++i;
                            continue;
                        }
                        for (k = i+1 ; k < n ; ++k) {
                            if (vec[k][j] == 0)
                                continue;
                            else if (vec[i][j] == vec[k][j]) {
                                vec[i][j] += vec[k][j];
                                vec[k][j] = 0;
                                i = k+1;
                                empty++;
                                break;
                            } else {
                                i = k;
                                break;
                            }
                        }
                        i = k;
                    }
                }
                // 向上移动方块
                for (j = 0 ; j < n ; ++j) {
                    for (i = 0 ; i < n ; ++i) {
                        if (vec[i][j] == 0)
                            continue;
                        for (k = i ; k > 0 && vec[k-1][j] == 0 ; --k) {
                            vec[k-1][j] = vec[k][j];
                            vec[k][j] = 0;
                            move = 1;
                        }
                    }
                }
                break;

            case 'S':
            case 's':
                // 从下到上消去相同方块
                for (j = 0 ; j < n ; ++j) {
                    for (i = n-1 ; i >= 0 ; ) {
                        if (vec[i][j] == 0) {
                            --i;
                            continue;
                        }
                        for (k = i-1 ; k >= 0 ; --k) {
                            if (vec[k][j] == 0)
                                continue;
                            else if (vec[i][j] == vec[k][j]) {
                                vec[i][j] += vec[k][j];
                                vec[k][j] = 0;
                                i = k-1;
                                empty++;
                                break;
                            } else {
                                i = k;
                                break;
                            }
                        }
                        i = k;
                    }
                }
                // 向下移动方块
                for (j = 0 ; j < n ; ++j) {
                    for (i = n-1 ; i >= 0 ; --i) {
                        if (vec[i][j] == 0)
                            continue;
                        for (k = i ; k < n-1 && vec[k+1][j] == 0 ; ++k) {
                            vec[k+1][j] = vec[k][j];
                            vec[k][j] = 0;
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
        if (empty <= 0) game_over();
        draw();

        // 生成新方块
        if (empty != old_empty || move == 1) {
            do {
                new_pos.first = rand() % n;
                new_pos.second = rand() % n;
            } while (vec[new_pos.first][new_pos.second] != 0);
            cnt_value(new_pos);
            do {
                tmp = rand() % n;
            } while (tmp == 0 || tmp == 2);
            vec[new_pos.first][new_pos.second] = tmp + 1;
            --empty;
            draw_one(new_pos);
        }
    }
}

// 初始化屏幕，并在方格中随机生成一个位置并放入数字2
void Game::init() {
    pair<int, int> pos;
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    srand(time(0));
    pos.first = rand() % n;
    pos.second = rand() % n;
    vec[pos.first][pos.second] = 2;
    --empty;
    old_pos = pos;      // 保存为旧位置
    draw();
}

int main() {
    int n;
    cout << "Enter 'A/a' to turn left"<< endl;
    cout << "Enter 'D/d' to turn right"<< endl;
    cout << "Enter 'W/w' to turn up"<< endl;
    cout << "Enter 'S/s' to turn down"<< endl;
    cout << endl;
    cout << "Now give me the board's size" << endl;
    cin >> n;
    cin.ignore(INT_MAX, '\n');
    Game game(n);
    game.init();
    game.play();
    endwin();
    return 0;
}

