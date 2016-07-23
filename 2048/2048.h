#include <cstdio>
#include <cstdlib>
#include <curses.h>
#include <ctime>
#include <unistd.h>
#include <signal.h>
#include <vector>
#include <utility>
#include <iostream>
#include <climits>

using std::vector;
using std::pair;
using std::cin;
using std::cout;
using std::endl;

class Game {
public:
    Game(int x);
    int game_over();
    int cnt_one(pair<int, int> &pos);
    void cnt_value(pair<int, int> &new_pos);
    void draw_one(pair<int, int> &pos);
    void draw();
    void play();
    void init();
private:
    int n;
    int empty;
    pair<int, int> old_pos;
    vector<vector<int>> vec;
};
